% 2010-08-10  Michele Tavella <michele.tavella@epfl.ch>
%
% MTPATH_CLEAN Clean directory name from trailing slashes
%    CPATH = MTPATH_CLEAN(PATH) 
%    
%    Remove trailing slashes to ease string comparison when using the path()
%    function.
%
%    Example:
%        mtpath_clean('~/some/dir///') return '~/some/dir'
% 
%    See also MTPATH_HAS.

function cpath = mtpath_clean(path)
	while(path(end) == '/')
		path = path(1:end-1);
	end
	cpath = path;
