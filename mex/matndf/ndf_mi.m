% Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
% Michele Tavella <michele.tavella@epfl.ch>
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.

function smr_online(arg0, arg1, arg2)

% For historical reasons this function accepts 3 arguments.
% Normally not needed, but the users might want to pass something
% when launching this function for debugging (and not within the loop)
if(nargin == 0) 
	arg0 = ''; 
	arg1 = ''; 
	arg2 = ''; 
end

% Include all the required toolboxes
ndf_include();

% Prepare and enter main loop
try 
	% Initialize loop structure
	ndf_loopnew;

	% Connect to the CnbiTk loop
	if(cl_connect(loop.cl) == false)
		disp('[ndf_checkloop] Cannot connect to CNBI Loop, killing matlab');
		exit;
	end	

	% Check the names
	% - if in format /name, then query the nameserver for the IPs/filenames
	% - otherwise, keep them as they are
	% - if pn, aD or aC are empty after calling ndf_checknames, their value
	%   will be set according to what is stored in the XML configuration
	% - also, if the nameserver query fails, pn, aD and aC will be empty and
	%   their values will be set according to the XML configuration
	loop = ndf_loopconfig(loop, 'mi');
	if(loop.cfg.config == 0)
		disp('[ndf_checkloop] Cannot retrieve loop configuration, killing matlab');
		exit;
	end
	loop = ndf_loopnames(loop);
	
	if(isempty(loop.cfg.ndf.pipe))
		disp('[ndf_checkloop] NDF configuration failed, killing matlab:');
		disp(['  Pipename:   "' loop.cfg.ndf.pipe '"']);
		disp(['  iC address: "' loop.cfg.ndf.ic '"']);
		disp(['  iD address: "' loop.cfg.ndf.id '"']);
		exit;
	end

	% -------------------------------------------------------------- %
	% User initialization                                            %
	% -------------------------------------------------------------- %
    user.ns.plot = cl_retrieveconfig(loop.cl, 'mi', 'plot');
    user.plot = strcmp(user.ns.plot, 'true');
	% -------------------------------------------------------------- %
	% /User initialization                                           %
	% -------------------------------------------------------------- %

	% Prepare NDF srtructure
	ndf.conf  = {};
	ndf.size  = 0;
	ndf.frame = ndf_frame();
	ndf.sink  = ndf_sink(loop.cfg.ndf.pipe);

	% -------------------------------------------------------------- %
	% User TOBI configuration                                        %
	% -------------------------------------------------------------- %
	% Configure TiD message
	idmessage_setevent(loop.mDo, 0);
	% Dump TiC/TiD messages
	icmessage_dumpmessage(loop.mC);
	idmessage_dumpmessage(loop.mDo);
	% -------------------------------------------------------------- %
	% /User TOBI configuration                                       %
	% -------------------------------------------------------------- %
	
	% Pipe opening and NDF configurationf
	% - Here the pipe is opened
	% - ... and the NDF ACK frame is received
	disp('[ndf_checkloop] Receiving ACK...');
	[ndf.conf, ndf.size] = ndf_ack(ndf.sink);
    
    % Read in parameters
    ndf.nTasks = ccfgtaskset_count(loop.cfg.taskset);
    for t = 0:ndf.nTasks-1
        task = ccfgtaskset_gettaskbyid(loop.cfg.taskset, t);
        ndf.tasklabel{t+1} = ccfgtask_getgdf(task);
        ndf.thresholds(t+1) = ccfgtask_getconfig_float(task, 'threshold');
    end
    ccfg_root(loop.cfg.config);
    ccfg_setbranch(loop.cfg.config);
    ndf.integration = ccfg_quickfloat(loop.cfg.config, 'online/mi/integration');
    if isnan(ndf.integration)
        disp('[smr_online] integration not found in XML file')
        disp('[smr_online] Killing MATLAB')
        exit;
    end
    ccfg_root(loop.cfg.config);
    ccfg_setbranch(loop.cfg.config);
    ndf.rejection = ccfg_quickfloat(loop.cfg.config, 'online/mi/rejection');
    if isnan(ndf.rejection)
        disp('[smr_online] rejection not found in XML file')
        disp('[smr_online] Killing MATLAB')
        exit;
    end

	cl_updatelog(loop.cl, sprintf('rejection=%f', ndf.rejection));
	cl_updatelog(loop.cl, sprintf('integration=%f', ndf.integration));

	% -------------------------------------------------------------- %
	% User EEG data configuration                                    %
	% -------------------------------------------------------------- %
    
	bci = eegc3_smr_newbci();
    try
        bci.analysis = load(loop.cfg.classifier.file);
    catch exception
        disp(['[smr_online] Exception: Classifier Not Found: ' exception.message ]);
        disp(exception);
        disp(exception.stack);
        disp('[smr_online] Killing Matlab...');
        exit;
    end
    bci.analysis = bci.analysis.analysis;
    bci.eeg = ndf_ringbuffer(bci.analysis.settings.eeg.fs, ...
                             bci.analysis.settings.eeg.chs, 1);
    bci.tri = ndf_ringbuffer(bci.analysis.settings.eeg.fs, 1, 1);
    bci.support = eegc3_smr_newsupport(bci.analysis, ndf.rejection, ndf.integration);
    bci.tim = ndf_ringbuffer(ndf.conf.sf/ndf.conf.samples, ndf.conf.tim_channels, 5.00);
    
    if user.plot
        eegc3_figure(1);
        subplot(10, 1, 1:3)
        user.h_eeg = imagesc(eegc3_car(eegc3_dc(bci.eeg))');
        user.h_eeg_title = ...
			title(sprintf('Frame=%6.6d, Dl=%7.2f (ms)', ndf.frame.index, loop.jump.toc));
        set(gca, 'XTickLabel', {});
        ylabel('eeg');

        user.h_teeg_axis = subplot(10, 1, 4:7);
        user.h_teeg = plot(bsxfun(@plus,0*bci.eeg,20*(1:size(bci.eeg,2))));

        subplot(10, 1, 8)
        user.h_tri = imagesc(bci.tri',[0 1]);
        ylabel('tri');
        set(gca, 'XTickLabel', {});
        set(gca, 'YTickLabel', {});

        user.h_tim_axis = subplot(10, 1, 9:10);
        user.h_tim = plot(bci.tim);
        ylabel('tim');
        set(gca, 'XTickLabel', {});
        user.h_tim_xlabel = xlabel(sprintf('Max delay %.2f ms', max(abs(bci.tim))));
        axis tight;
        drawnow;
    end
    
	% -------------------------------------------------------------- %
	% /User EEG data configuration                                   %
	% -------------------------------------------------------------- %
	
	% NDF ACK check
	% - The NDF id describes the acquisition module running
	% - Bind your modules to a particular configuration (if needed)
	disp(['[ndf_checkloop] NDF type id: ' num2str(ndf.conf.id)]);
    
	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	disp('[ndf_checkloop] Receiving NDF frames...');
	loop.jump.tic = ndf_tic();
    
	while(true)
		% Read NDF frame from pipe
		loop.jump.toc = ndf_toc(loop.jump.tic);
		loop.jump.tic = ndf_tic();
		[ndf.frame, ndf.size] = ndf_read(ndf.sink, ndf.conf, ndf.frame);

		% Acquisition is down, exit
		if(ndf.size == 0)
			disp('[ndf_checkloop] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		% - If you want to keep track of the timestamps for debugging, 
		%   update buffer.tim in this way:
		%	buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		bci.tim = ndf_add2buffer(bci.tim, ndf_toc(ndf.frame.timestamp));
		bci.eeg = ndf_add2buffer(bci.eeg, ndf.frame.eeg);
		bci.tri = ndf_add2buffer(bci.tri, ndf.frame.tri);
        
		% -------------------------------------------------------------- %
		% User main loop                                                 %
		% -------------------------------------------------------------- %
        
        % plot the data
        if user.plot
            set(user.h_eeg,'CData',eegc3_car(eegc3_dc(bci.eeg))')
            set(user.h_eeg_title,'String', ...
                sprintf('Frame=%6.6d, Dl=%7.2f (ms)',ndf.frame.index, loop.jump.toc))
            for i=1:length(user.h_teeg)
                set(user.h_teeg(i),'YData',bci.eeg(:,i)+20*i)
            end
            axis(user.h_teeg_axis,[1 size(bci.eeg,1) 0 20*(size(bci.eeg,2)+1)])
            set(user.h_tri,'CData',bci.tri')
            set(user.h_tim,'YData',bci.tim)
            set(user.h_tim_xlabel,'String',sprintf('Max delay %.2f ms', max(abs(bci.tim))))
            axis(user.h_tim_axis,[1 length(bci.tim) floor(min(bci.tim)) ceil(max(bci.tim))])
            drawnow
        end
        
        [bci.support, tmp.nfeat] = ...
            eegc3_smr_classify(bci.analysis, bci.eeg, bci.support);
        
		% Handle async TOBI iD communication
		if(tid_isattached(loop.iD) == true)
			while(tid_getmessage(loop.iD, loop.sDi) == true)
                id_event = idmessage_getevent(loop.mDi);
                disp(num2str(id_event))
                if id_event == 781 || id_event == 897 || id_event == 898
					bci.support.nprobs = ones(size(bci.support.nprobs)) ...
						/ length(bci.support.nprobs);
                end
			end
		else
			tid_attach(loop.iD, loop.cfg.ndf.id);
        end
		
		% Handle sync TOBI iC communication
		if(tic_isattached(loop.iC) == true)
			for t = 1:ndf.nTasks
                % Map the tasklabel to the probabilities output from EEGC3
                taskloc = find(ndf.tasklabel{t} == bci.analysis.settings.task.classes_old);
                if length(taskloc) ~= 1
                    disp('[smr_online] Task not present in classifier')
                    disp('[smr_online] Killing MATLAB')
                    exit;
                end
				icmessage_setvalue(loop.mC, loop.cfg.classifier.id, ...
					num2str(ndf.tasklabel{t}), bci.support.nprobs(taskloc));
			end
			tic_setmessage(loop.iC, loop.sC, ndf.frame.index);
		else
			tic_attach(loop.iC, loop.cfg.ndf.ic);
        end
        
		% -------------------------------------------------------------- %
		% /User main loop                                                %
		% -------------------------------------------------------------- %

		% Check if module is running slow
		loop.jump = ndf_jump_update(loop.jump, ndf.frame.index);
		if(loop.jump.isjumping)
			disp('[ndf_checkloop] Error: running slow');
			break;
        end
        
	end
catch exception
	ndf_printexception(exception);
	disp('[ndf_checkloop] Going down');
	loop.exit = true;
end

try 
	% Tear down loop structure
	ndf_loopdelete;
catch exception
	ndf_printexception(exception);
	loop.exit = true;
end

disp(['[ndf_checkloop] Loop uptime: ' ...
	num2str(ndf_toc(loop.uptime)/1000/60) ...
	' minutes']);

disp('[ndf_checkloop] Killing Matlab...');
if(loop.exit == true)
	exit;
end
