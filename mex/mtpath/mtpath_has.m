% 2010-05-15  Michele Tavella <michele.tavella@epfl.ch>
%
% MTPATH_HAS Check if a directory is already in the Matlab path
%    MTPATH_HASPATH(SUBPATH) returns 1 if SUBPATH is already present in the
%    Matlab path, 0 otherwise.
% 
%    See also MTPATH_INCLUDE, MTPATH_CLEAN.

function status = mtpath_has(subpath)

% Inform the user if added correctly
if(isempty(strfind(path, subpath)))
	status = 0;
else
	status = 1;
end
