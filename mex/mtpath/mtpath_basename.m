% 2010-08-10  Michele Tavella <michele.tavella@epfl.ch>
%
% MTPATH_BASENAME Add directory to Matlab path
%    [BASENAME BASEPATH] = MTPATH_BASENAME(PATH) returns the basename and the
%    basepath (hope the name is correct :-) for the directory PATH.
%    
%    Example:
%        mtpath_basename('~/some//path//') returns ['~/some' 'path']
% 
%    See also MTPATH_HAS, MTPATH_CLEAN, MTPATH_INCLUDE.

function [basename basepath] = mtpath_basename(path)

path = mtpath_clean(path);
slashes = strfind(path, '/');
basename = path(slashes(end)+1:end);
basepath = strrep(path, basename, '');
basepath = mtpath_clean(basepath);
