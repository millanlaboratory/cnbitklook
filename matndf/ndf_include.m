%   Copyright (C) 2011 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of matndf
%
%   The libndf library is free software: you can redistribute it and/or
%   modify it under the terms of the version 3 of the GNU General Public
%   License as published by the Free Software Foundation.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program.  If not, see <http://www.gnu.org/licenses/>.
%
%   function ndf_include()
function ndf_include()

try
	if(isempty(getenv('MTPATH_ROOT')))
		disp('[ndf_include] $MTPATH_ROOT not found, using default');
		setenv('MTPATH_ROOT', '/opt/mtpath/');
	end
	if(isempty(getenv('CNBITK_MATLAB')))
		disp('[ndf_include] $CNBITK_MATLAB not found, using default');
		setenv('CNBITK_MATLAB', '/opt/cnbitkmat/');
	end
	if(isempty(getenv('EEGC3_ROOT')))
		disp('[ndf_include] $EEGC3_ROOT not found, using default');
		setenv('EEGC3_ROOT', '/opt/eegc3');
	end

	addpath(getenv('MTPATH_ROOT'));
	if(isempty(which('mtpath_include')))
		disp('[ndf_include] mtpath not installed, killing Matlab');
		exit;
	end
	mtpath_include('$CNBITK_MATLAB/');
	mtpath_include('$EEGC3_ROOT/');
	mtpath_include('$EEGC3_ROOT/modules/smr');
catch exception
	disp(['[ndf_include] Exception: ' exception.message ]);
	disp(exception);
	disp(exception.stack);
	disp('[ndf_include] Killing Matlab...');
	exit;
end
