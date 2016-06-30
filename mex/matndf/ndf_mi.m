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

function ndf_mi(arg0, arg1, arg2)

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
		disp('[ndf_mi] Cannot connect to CNBI Loop, killing matlab');
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
		disp('[ndf_mi] Cannot retrieve loop configuration, killing matlab');
		exit;
	end
	loop = ndf_loopnames(loop);
	
	if(isempty(loop.cfg.ndf.pipe))
		disp('[ndf_mi] NDF configuration failed, killing matlab:');
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
	disp('[ndf_mi] Receiving ACK...');
	[ndf.conf, ndf.size] = ndf_ack(ndf.sink);
    
    % Read in parameters
    user.nTasks = ccfgtaskset_count(loop.cfg.taskset);
    for t = 0:user.nTasks-1
        task = ccfgtaskset_gettaskbyid(loop.cfg.taskset, t);
        user.tasklabel{t+1} = ccfgtask_getgdf(task);
        user.thresholds(t+1) = ccfgtask_getconfig_float(task, 'threshold');
    end
    ccfg_root(loop.cfg.config);
    ccfg_setbranch(loop.cfg.config);
    user.integration = ccfg_quickfloat(loop.cfg.config, 'online/mi/integration');
    if isnan(user.integration)
        disp('[ndf_mi] integration not found in XML file')
        disp('[ndf_mi] Killing MATLAB')
        exit;
    end
    ccfg_root(loop.cfg.config);
    ccfg_setbranch(loop.cfg.config);
    user.rejection = ccfg_quickfloat(loop.cfg.config, 'online/mi/rejection');
    if isnan(user.rejection)
        disp('[ndf_mi] rejection not found in XML file')
        disp('[ndf_mi] Killing MATLAB')
        exit;
    end

	cl_updatelog(loop.cl, sprintf('classifier=%s', loop.cfg.classifier.file));
	cl_updatelog(loop.cl, sprintf('rejection=%f', user.rejection));
	cl_updatelog(loop.cl, sprintf('integration=%f', user.integration));
	msg_thresholds_log = 'thresholds=(';
	for t = 0:user.nTasks-1
	    msg_thresholds_log = sprintf('%s %f', msg_thresholds_log, user.thresholds(t+1));
        end
	msg_thresholds_log = sprintf('%s )', msg_thresholds_log);
	cl_updatelog(loop.cl, msg_thresholds_log);

	% -------------------------------------------------------------- %
	% User EEG data configuration                                    %
	% -------------------------------------------------------------- %
	user.bci = eegc3_smr_newbci();
	user.classifier = [loop.cfg.ns.path '/' loop.cfg.classifier.file];
    try
        user.bci.analysis = load(user.classifier);
		user.bci.analysis = user.bci.analysis.analysis;
		user.bci.support = eegc3_smr_newsupport(user.bci.analysis, ...
			user.rejection, user.integration);
    catch exception
		ndf_printexception(exception);
        disp('[ndf_mi] Killing Matlab...');
        exit;
    end

	if(user.bci.analysis.settings.eeg.chs ~= ndf.conf.eeg_channels)
		disp('[ndf_mi] Error: NDF channels differ from classifier settings');
        disp('[ndf_mi] Killing Matlab...');
        exit;
	end
	
	if(user.bci.analysis.settings.eeg.fs ~= ndf.conf.sf)
		disp('[ndf_mi] Error: NDF sample rate differs from classifier settings');
        disp('[ndf_mi] Killing Matlab...');
        exit;
	end

	buffer.eeg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.eeg_channels, 1.00);
	%buffer.exg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.exg_channels, 1.00);
	buffer.tri = ndf_ringbuffer(ndf.conf.sf, ndf.conf.tri_channels, 1.00);
	buffer.tim = ndf_ringbuffer(ndf.conf.sf/ndf.conf.samples, ...
		ndf.conf.tim_channels, 5.00);

    
    if(user.plot == true)
        eegc3_figure(1);
        subplot(10, 1, 1:3)
        user.h_eeg = imagesc(eegc3_car(eegc3_dc(buffer.eeg))');
		user.h_eeg_title = title(sprintf('Frame=%6.6d, Dl=%7.2f (ms)', ...
			ndf.frame.index, loop.jump.toc));
        set(gca, 'XTickLabel', {});
        ylabel('eeg');

        user.h_teeg_axis = subplot(10, 1, 4:7);
        user.h_teeg = plot(bsxfun(@plus,0*buffer.eeg,20*(1:size(buffer.eeg,2))));

        subplot(10, 1, 8)
        user.h_tri = imagesc(buffer.tri',[0 1]);
        ylabel('tri');
        set(gca, 'XTickLabel', {});
        set(gca, 'YTickLabel', {});

        user.h_tim_axis = subplot(10, 1, 9:10);
        user.h_tim = plot(buffer.tim);
        ylabel('tim');
        set(gca, 'XTickLabel', {});
        user.h_tim_xlabel = xlabel(sprintf('Max delay %.2f ms', max(abs(buffer.tim))));
        axis tight;
        drawnow;
    end
	% -------------------------------------------------------------- %
	% /User EEG data configuration                                   %
	% -------------------------------------------------------------- %
	
	% NDF ACK check
	% - The NDF id describes the acquisition module running
	% - Bind your modules to a particular configuration (if needed)
	disp(['[ndf_mi] NDF type id: ' num2str(ndf.conf.id)]);
    
	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	disp('[ndf_mi] Receiving NDF frames...');
	loop.jump.tic = ndf_tic();
    
	while(true)
		% Read NDF frame from pipe
		loop.jump.toc = ndf_toc(loop.jump.tic);
		loop.jump.tic = ndf_tic();
		[ndf.frame, ndf.size] = ndf_read(ndf.sink, ndf.conf, ndf.frame);

		% Acquisition is down, exit
		if(ndf.size == 0)
			disp('[ndf_mi] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		% - If you want to keep track of the timestamps for debugging, 
		%   update buffer.tim in this way:
		%	buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		buffer.eeg = ndf_add2buffer(buffer.eeg, ndf.frame.eeg);
		%buffer.exg = ndf_add2buffer(buffer.exg, ndf.frame.exg);
		buffer.tri = ndf_add2buffer(buffer.tri, ndf.frame.tri);
        
		% -------------------------------------------------------------- %
		% User main loop                                                 %
		% -------------------------------------------------------------- %
        
        % plot the data
        if(user.plot == true)
            set(user.h_eeg,'CData',eegc3_car(eegc3_dc(buffer.eeg))')
            set(user.h_eeg_title,'String', ...
                sprintf('Frame=%6.6d, Dl=%7.2f (ms)',ndf.frame.index, loop.jump.toc))
            for i=1:length(user.h_teeg)
                set(user.h_teeg(i),'YData',buffer.eeg(:,i)+20*i)
            end
            axis(user.h_teeg_axis,[1 size(buffer.eeg,1) 0 20*(size(buffer.eeg,2)+1)])
            set(user.h_tri,'CData',buffer.tri')
            set(user.h_tim,'YData',buffer.tim)
            set(user.h_tim_xlabel,'String',sprintf('Max delay %.2f ms', max(abs(buffer.tim))))
            axis(user.h_tim_axis,[1 length(buffer.tim) floor(min(buffer.tim)) ceil(max(buffer.tim))])
            drawnow
        end
        
        [user.bci.support, tmp.nfeat] = ...
            eegc3_smr_classify(user.bci.analysis, buffer.eeg, user.bci.support);
        
		% Handle async TOBI iD communication
		if(tid_isattached(loop.iD) == true)
			while(tid_getmessage(loop.iD, loop.sDi) == true)
                id_event = idmessage_getevent(loop.mDi);
                
				if(id_event == 781)
					user.bci.support.nprobs = ones(size(user.bci.support.nprobs)) ...
						/ length(user.bci.support.nprobs);
					printf('[ndf_mi] Resetting NDF=%d/iD=%d\n', ...
						ndf.frame.index, idmessage_getbidx(loop.mDi));
                end
			end
		else
			tid_attach(loop.iD, loop.cfg.ndf.id);
        end
		
		% Handle sync TOBI iC communication
		if(tic_isattached(loop.iC) == true)
			for t = 1:user.nTasks
                % Map the tasklabel to the probabilities output from EEGC3
                taskloc = find(user.tasklabel{t} == user.bci.analysis.settings.task.classes_old);
                if length(taskloc) ~= 1
                    disp('[ndf_mi] Task not present in classifier')
                    disp('[ndf_mi] Killing MATLAB')
                    exit;
                end
				icmessage_setvalue(loop.mC, loop.cfg.classifier.id, ...
					num2str(user.tasklabel{t}), user.bci.support.nprobs(taskloc));
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
			disp('[ndf_mi] Error: running slow');
			break;
        end
        
	end
catch exception
	ndf_printexception(exception);
	disp('[ndf_mi] Going down');
	loop.exit = true;
end

try 
	% Tear down loop structure
	ndf_loopdelete;
catch exception
	ndf_printexception(exception);
	loop.exit = true;
end

disp(['[ndf_mi] Loop uptime: ' ...
	num2str(ndf_toc(loop.uptime)/1000/60) ...
	' minutes']);

disp('[ndf_mi] Killing Matlab...');
if(loop.exit == true)
	exit;
end
