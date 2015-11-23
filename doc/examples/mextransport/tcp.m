%   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of the mextransport wrapper
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

socket = tr_new();
tr_init_socket_default(socket);
tr_tcpclient(socket);
tr_open(socket);

while(tr_connected(socket) < 0) 
	disp('Trying to connect...');
	status = tr_connect(socket, '127.0.0.1', '8000');
	if(status < 0)
		pause(0.50);
	end
end
[local.ip, local.port] = tr_getlocal(socket);
[remote.ip, remote.port] = tr_getremote(socket);

fprintf(1, 'Connected: %s:%d-->%s:%d\n', ...
	local.ip, local.port, remote.ip, remote.port);


for i = 1:1000
	tr_connected(socket)
	if(tr_connected(socket) < 0) 
		disp('Server dropped');
		break;
	end
	sentbytes = tr_send(socket, 'Hello from Matlab');
	recvbytes = tr_recv(socket);
	if(recvbytes)
		disp(['Received: ' tr_getbuffer(socket)]);
	end
	%pause(0.1);
end

disp('Closing');
tr_close(socket);
tr_free(socket);
tr_delete(socket);
