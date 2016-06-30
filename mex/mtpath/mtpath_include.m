% 2010-05-15  Michele Tavella <michele.tavella@epfl.ch>
%
% MTPATH_INCLUDE Add directory to Matlab path
%    STATUS = MTPATH_INCLUDE(NEWPATH, RECURSIVE) adds the directory specified by
%    NEWPATH, if RECURSIVE is set to 'true', then all the subdirectories are
%    added to the path as well.
%    
%    If a file named as BASENAME_init.m is found, then it is executed
%    automatically. For example, if you add '~/whatever', and
%    '~/whatever_init.m" is found, then 'whatever_init' is called automatically.
%    This is done to ease recursive inclusions.
% 
%    If NEWPATH is already in the Matlab path, no action is performed, although 
%    BASENAME_init is called if found.
%    In case of error, a message is shown and 0 is returned; 1 otherwise.
%
%    Example:
%        mtpath_include('~/path/to/my/toolbox/')
%        mtpath_include('$PATH_TO_MY_TOOLBOX/')
%        mtpath_include('$PATH_TO_MY_TOOLBOXES/', true)
% 
%    See also MTPATH_HAS, MTPATH_BASENAME.

function status = mtpath_include(newpath, recursive, maxlevel)

if(nargin < 2)
	recursive = false;
end
if(nargin < 3)
    maxlevel = 0;
end

% If newpath starts with '~', replace with $HOME
newpath = strrep(newpath, '~', getenv('HOME'));
% If newpath ends with '/[//]', clean them all
newpath = mtpath_clean(newpath);

newpath = strrep(newpath, '///', '/');
newpath = strrep(newpath, '//', '/');

% Solve all the envvars
chunks = regexp(newpath, '\/', 'split');
if(length(chunks) > 0)
	for i = 1:length(chunks)
		chunk = chunks{i};
		if(isempty(chunk) == false)
			if(chunk(1) == '$')
				schunk = getenv(chunk(2:end));
				if(isempty(schunk))
					disp(['[mtpath_include] Error: envvar not defined: ' chunk]);
					status = 0;
					return;
				else
					newpath = strrep(newpath, chunk, schunk);
				end
			end
		end
	end
end

% Check if directory exists
if(exist(newpath, 'dir') == 0)
	disp(['[mtpath_include] Error: directory not found: ' newpath]);
	status = 0;
	return;
else
	status = 1;
end

% Inform the user if added correctly
if(mtpath_has(newpath) == false)
	disp(['[mtpath_include] Adding: ' newpath]);
end

% Add path
if(recursive)
	addpath(genpath(newpath));
else
	addpath(newpath);
end

% Init path
mtpath_init(newpath, maxlevel);
