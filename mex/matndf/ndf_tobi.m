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
function tobi = ndf_tobi(loop)
tobi = {};

% Configure TCP endpoints for TiD 
tobi.iD.socket  	= [];
tobi.iD.address 	= '127.0.0.1:9000';
tobi.iD.ipport  	= {'127.0.0.1', '9000'};
tobi.iD.message     = loop.mD;
tobi.iD.serializer 	= loop.sD;
tobi.iD.buffer 		= '';
tobi.iD.cache 		= '';
tobi.iD.hdr 		= '<tobiid';
tobi.iD.trl 		= '/>';
tobi.iD.queue 		= {};
	
% Configure TCP endpoints for TiC
tobi.iC.socket  	= [];
tobi.iC.address 	= '127.0.0.1:9500';
tobi.iC.ipport  	= {'127.0.0.1', '9500'};
tobi.iC.message	    = loop.mC;
tobi.iC.serializer  = loop.sC;
tobi.iC.cache 		= '';
tobi.iC.hdr 		= '<tobiic';
tobi.iC.trl 		= '</tobiic>';

tobi.iD.address = loop.cfg.ndf.id;
tobi.iD.ipport 	= regexp(tobi.iD.address, ':', 'split');
disp(['[ndf_tobi] TOBI iD endpoint: ' tobi.iD.address]);

tobi.iC.address = loop.cfg.ndf.ic;
tobi.iC.ipport 	= regexp(tobi.iC.address, ':', 'split');
disp(['[ndf_tobi] TOBI iC endpoint: ' tobi.iC.address]);

% iD socket
% - Create a new TCP socket for sending/receiving events
tobi.iD.socket = loop.nC;
tr_init_socket_default(tobi.iD.socket);
tr_tcpclient(tobi.iD.socket);
tr_open(tobi.iD.socket);

% iC socket
% - Create a new TCP socket for sending classifier outputs
tobi.iC.socket = loop.nD;
tr_init_socket_default(tobi.iC.socket);
tr_tcpclient(tobi.iC.socket);
tr_open(tobi.iC.socket);
