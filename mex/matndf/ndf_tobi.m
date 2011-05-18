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
function tobi = ndf_tobi(addressD, addressC, messageC, messageD)

tobi = {};

% Configure TCP endpoints for TiD 
tobi.iD.socket  	= [];
tobi.iD.address 	= '127.0.0.1:9000';
tobi.iD.ipport  	= {'127.0.0.1', '9000'};
tobi.iD.message     = messageD;
tobi.iD.serializer 	= idserializerrapid_new(tobi.iD.message);
tobi.iD.buffer 		= '';
tobi.iD.cache 		= '';
tobi.iD.hdr 		= '<tobiid';
tobi.iD.trl 		= '/>';
tobi.iD.queue 		= {};
	
% Configure TCP endpoints for TiC
tobi.iC.socket  	= [];
tobi.iC.address 	= '127.0.0.1:9500';
tobi.iC.ipport  	= {'127.0.0.1', '9500'};
tobi.iC.message	    = messageC;
tobi.iC.serializer  = icserializerrapid_new(tobi.iC.message);
tobi.iC.cache 		= '';
tobi.iC.hdr 		= '<tobiic';
tobi.iC.trl 		= '</tobiic>';

if(nargin >= 2 | isempty(addressD) == false)
	tobi.iD.address = addressD;
	tobi.iD.ipport 	= regexp(tobi.iD.address, ':', 'split');
end
disp(['[ndf_tobi] TOBI iD endpoint: ' tobi.iD.address]);

if(nargin >= 3 | isempty(addressC) == false)
	tobi.iC.address = addressC;
	tobi.iC.ipport 	= regexp(tobi.iC.address, ':', 'split');
end
disp(['[ndf_tobi] TOBI iC endpoint: ' tobi.iC.address]);

% iD socket
% - Create a new TCP socket for sending/receiving events
tobi.iD.socket = tr_new();
tr_init_socket_default(tobi.iD.socket);
tr_tcpclient(tobi.iD.socket);
tr_open(tobi.iD.socket);

% iC socket
% - Create a new TCP socket for sending classifier outputs
tobi.iC.socket = tr_new();
tr_init_socket_default(tobi.iC.socket);
tr_tcpclient(tobi.iC.socket);
tr_open(tobi.iC.socket);
