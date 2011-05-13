%   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of the mexcnbiconfig wrapper
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

clear all;
if(mtpath_include('/opt/mextobiic') == 0)
	mtpath_include('$CNBITK_MATLAB/mextobiic');
end

config = ccfg_new();
ccfg_importfile(config, '../extra/cnbiconfig_example.xml');

% Allocate ICMessage and ICSerializerRapid wrappers
ic.message = icmessage_new();
ic.serializer = icserializerrapid_new(ic.message);

taskset = ccfg_onlinem(config, 'mi', 'mi_rhlh', ic.message);

if(taskset == 0)
	disp('Error: taskset not found');
	return;
end

[ic.classifier.name, ic.classifier.description, ic.classifier.filename] = ...
	ccfgtaskset_getclassifier(taskset);
[ndf.function, ndf.pipename, ndf.id, ndf.ic] = ...
	ccfgtaskset_getndf(taskset);

tasktot = ccfgtaskset_count(taskset);
fprintf(1, 'Taskset has %d tasks\n', tasktot);
tasks = {};
labels = {};
for i = 1:tasktot
	tasks{i} = ccfgtaskset_gettaskbyid(taskset, i-1);
	labels{i} = ccfgtask_getname(tasks{i});
end

% Simulate online loop, in which we map a cprobs vector to ICMessage
cprobs = zeros(1, tasktot);
for i = 1:1000
	cprobs = rand(1, tasktot);
	cprobs = cprobs ./ sum(cprobs);
	for t = 1:tasktot
		icmessage_setvalue(ic.message, ic.classifier.name, labels{t}, cprobs(t));
		icmessage_setvalue(ic.message, ic.classifier.name, labels{t}, cprobs(t));
	end
	buffer = icmessage_serialize(ic.serializer);
	fprintf(1, 'It %d: %s\n', i, buffer);
	%icmessage_dumpmessage(ic.message);
end
	

% Free ICMessage and ICSerializerRapid wrappers
icmessage_delete(ic.message);
icserializerrapid_delete(ic.serializer);

