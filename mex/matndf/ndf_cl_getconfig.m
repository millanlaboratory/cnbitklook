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
function cfg = ndf_cl_getconfig(cl, name, pn, aD, aC);

if(nargin < 3); pn = ''; end
if(nargin < 4); aD = ''; end
if(nargin < 5); aC = ''; end

% Retrieve from nameserver:
% - block		the XML block (i.e. mi)
% - taskset		the XML taskset (i.e. mi_rhlh)
% - xml			the XML filename
% - path		the working directory
cfg = {};
cfg.ns.modality = cl_retrieveconfig(cl, name, 'modality');
cfg.ns.block    = cl_retrieveconfig(cl, name, 'block');
cfg.ns.taskset  = cl_retrieveconfig(cl, name, 'taskset');
cfg.ns.xml      = cl_retrieveconfig(cl, name, 'xml');
cfg.ns.path     = cl_retrieveconfig(cl, name, 'path');

cfg.config = ccfg_new();
if(ccfg_importfile(cfg.config, cfg.ns.xml) == 0)
	disp('[ndf_cl_config] Cannot load XML file, killing matlab');
	exit;
end

% Use ccfg_* to load the XML and retrieve the taskset. 
cfg.messageC = icmessage_new();
cfg.messageD = idmessage_new();
cfg.taskset = ccfg_online(cfg.config, cfg.ns.block, cfg.ns.taskset, ...
	cfg.messageC, cfg.messageD);
if(cfg.taskset == 0)	
	disp('[ndf_cl_config] Cannot retrieve taskset, killing matlab');
	exit;
end

% The taskset contains informations about:
% - the classifier
% - the NDF configuration (pipes etc)
[cfg.classifier.name, cfg.classifier.desc, cfg.classifier.file] = ...
	ccfgtaskset_getclassifier(cfg.taskset);
[cfg.ndf.exec, cfg.ndf.pipe, cfg.ndf.id, cfg.ndf.ic] = ...
	ccfgtaskset_getndf(cfg.taskset);


% Override NDF option from XML if needed
if(isempty(aC) == false); 
	disp('[ndf_cl_config] Overriding iC address');
	cfg.ndf.ic = aC;
end
if(isempty(aD) == false); 
	disp('[ndf_cl_config] Overriding iD address');
	cfg.ndf.id = aD;
end
if(isempty(pn) == false); 
	disp('[ndf_cl_config] Overriding pipename');
	cfg.ndf.pipe = pn;
end

fprintf(1, '[ndf_cl_config] Nameserver configuration:\n');
fprintf(1, '  Modality: %s\n', cfg.ns.modality);
fprintf(1, '  Block:    %s\n', cfg.ns.block);
fprintf(1, '  Taskset:  %s\n', cfg.ns.taskset);
fprintf(1, '  XML:      %s\n', cfg.ns.xml);
fprintf(1, '  Path:     %s\n', cfg.ns.path);
fprintf(1, '[ndf_cl_config] NDF configuration:\n');
fprintf(1, '  Exec:     %s\n', cfg.ndf.exec);
fprintf(1, '  Pipe:     %s\n', cfg.ndf.pipe);
fprintf(1, '  TOBI iD:  %s\n', cfg.ndf.id);
fprintf(1, '  TOBI iC:  %s\n', cfg.ndf.ic);
fprintf(1, '[ndf_cl_config] Classifier configuration:\n');
fprintf(1, '  Name      %s\n', cfg.classifier.name);
fprintf(1, '  Desc.:    %s\n', cfg.classifier.desc);
fprintf(1, '  Filename: %s\n', cfg.classifier.file);
