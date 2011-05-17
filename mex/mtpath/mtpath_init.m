% 2010-05-15  Michele Tavella <michele.tavella@epfl.ch>
%
% MTPATH_INIT Calls BASENAME_init function
%    If a file named as BASENAME_init.m is found, then it is executed
%    automatically. For example, if you add '~/whatever', and
%    '~/whatever_init.m" is found, then 'whatever_init' is called automatically.
%    This is done to ease recursive inclusions.
% 
%    See also MTPATH_BASENAME, MTPATH_INCLUDE.

function mtpath_init(newpath, maxlevel)

if(nargin < 2)
    maxlevel = 2;
end

if(nargin == 0)
	newpath = pwd;
end

[basename basepath] = mtpath_basename(newpath);
initfile = [newpath '/' basename '_init.m'];
initfunc = [basename '_init'];
if(exist(initfile, 'file'))
	if(mtpath_has(newpath) == false)
		disp(['[mtpath_init] Init file: ' initfile]);
	end
	eval(initfunc);
	return;
end

if(maxlevel <= 0)
    return;
end

chunks = strsplit('/', newpath);
for i = length(chunks):-1:1
	initfile = [chunks{i} '_init.m'];
	initfunc = [chunks{i} '_init'];
    level = length(chunks)-i;
    if(level >= maxlevel)
        return;
    end
	if(exist(initfile, 'file'))
		if(mtpath_has(newpath) == false)
			disp(['[mtpath_init] Init file (guessing at level -' ...
				num2str(level) '): ' initfile]);
		end
		eval(initfunc);
		return;
	end
end

function parts = strsplit(splitstr, str, option)
	nargsin = nargin;
	error(nargchk(2, 3, nargsin));
	if nargsin < 3
		option = 'omit';
   else
	   option = lower(option);
   end
   splitlen = length(splitstr);
   parts = {};
   while 1
	   k = strfind(str, splitstr);
	   if isempty(k)
		   parts{end+1} = str;
		   break
	  end
	  switch option
	  case 'include'
			parts(end+1:end+2) = {str(1:k(1)-1), splitstr};
		case 'append'
			parts{end+1} = str(1 : k(1)+splitlen-1);
		case 'omit'
			parts{end+1} = str(1 : k(1)-1);
		otherwise
			error(['Invalid option string -- ', option]);
	  end
	  str = str(k(1)+splitlen : end);
  end
