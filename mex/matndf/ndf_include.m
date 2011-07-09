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
function ndf_include()

try
	if(isempty(getenv('CNBITKMAT_ROOT')))
		disp('[ndf_include] $CNBITKMAT_ROOT not found, using default');
		setenv('CNBITKMAT_ROOT', '/usr/share/cnbiloop/cnbitkmat/');
	end
	if(isempty(getenv('EEGC3_ROOT')))
		disp('[ndf_include] $EEGC3_ROOT not found, using default');
		setenv('EEGC3_ROOT', '/opt/eegc3');
	end

	mtpath_root = [getenv('CNBITKMAT_ROOT') '/mtpath'];
	if(exist(mtpath_root, 'dir'))
		addpath(mtpath_root);
	end

	if(isempty(which('mtpath_include')))
		disp('[ndf_include] mtpath not installed, killing Matlab');
		exit;
	end
	mtpath_include('$CNBITKMAT_ROOT/');
	mtpath_include('$EEGC3_ROOT/');
	mtpath_include('$EEGC3_ROOT/modules/smr');
catch exception
	disp(['[ndf_include] Exception: ' exception.message ]);
	disp(exception);
	disp(exception.stack);
	disp('[ndf_include] Killing Matlab...');
	exit;
end
