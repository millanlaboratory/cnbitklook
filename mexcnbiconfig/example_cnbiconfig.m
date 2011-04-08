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
addpath('../mextobiic');

config = ccfg_new('../libcnbiconfig/extra/example.xml');

ccfg_root(config);
ccfg_setbranch(config);
ccfg_branch(config);

ccfg_root(config);
device = ccfg_quickstring(config, 'options/fes/dev');
fprintf(1, 'Device: %s\n', device);

ccfg_root(config);
fprintf(1, 'Viscosity: %f\n', ...
	ccfg_quickfloat(config, 'parameters/copilot/viscosity'));
ccfg_root(config);
fprintf(1, 'Threshold: %f\n', ...
	ccfg_quickfloat(config, 'parameters/copilot/threshold'));

ccfg_root(config);
fprintf(1, 'Shared control: %d\n', ...
	ccfg_quickbool(config, 'options/robotino/sharedcontrol'));

mOnlineMI = icmessage_new();
mOnlineErrP = icmessage_new();
sOnlineMI = icserializerrapid_new(mOnlineMI);
sOnlineErrP = icserializerrapid_new(mOnlineErrP);

[tOnlineMI, config] = ccfg_onlinem(config, 'task1', 'mi_rhlh', ...
	mOnlineMI);
if(tOnlineMI == 0)
	fprintf(1, 'Not found\n');
	return;
end

[tOnlineErrP, config] = ccfg_onlinem(config, 'task1', 'errp', ...
	mOnlineErrP);
if(tOnlineMI == 0)
	fprintf(1, 'Not found\n');
	return;
end


taskA = [];
taskB = [];
taskC = [];
if(ccfgtaskset_hastask(tOnlineMI, 'mi_hand_right'))
	taskA = ccfgtaskset_gettask(tOnlineMI, 'mi_hand_right');
	nameA = ccfgtask_getname(taskA);
	descA = ccfgtask_getdescription(taskA);
	idA = ccfgtask_getid(taskA);
	trialsA = ccfgtask_gettrials(taskA);
	hwtA = ccfgtask_gethwt(taskA);
	gdfA = ccfgtask_getgdf(taskA);
	fprintf(1, 'Task %s, Description %s, ID %d, Trials %d, HWT %d, GDF %d\n', ...
		nameA, descA, idA, trialsA, hwtA, gdfA);
end
if(ccfgtaskset_hastask(tOnlineMI, 'mi_hand_left'))
	taskB = ccfgtaskset_gettask(tOnlineMI, 'mi_hand_left');
	nameB = ccfgtask_getname(taskB);
	descB = ccfgtask_getdescription(taskB);
	idB = ccfgtask_getid(taskB);
	trialsB = ccfgtask_gettrials(taskB);
	hwtB = ccfgtask_gethwt(taskB);
	gdfB = ccfgtask_getgdf(taskB);
	fprintf(1, 'Task %s, Description %s, ID %d, Trials %d, HWT %d, GDF %d\n', ...
		nameB, descB, idB, trialsB, hwtB, gdfB);
end
if(ccfgtaskset_gettask(tOnlineMI, 'blah'))
	taskC = ccfgtaskset_hastask(tOnlineMI, 'blah')
end

messageMI = icmessage_serialize(mOnlineMI, sOnlineMI);
messageErrP = icmessage_serialize(mOnlineErrP, sOnlineErrP);
fprintf(1, 'ICMessage for MI: %s\n', messageMI);
fprintf(1, 'ICMessage for ErrP: %s\n', messageErrP);
	
mOnlineAll = icmessage_new();
sOnlineAll = icserializerrapid_new(mOnlineAll);
[tOnlineMI2, config] = ccfg_onlinem(config, 'task1', 'mi_rhlh', ...
	mOnlineAll);
[tOnlineErrP, config] = ccfg_onlinem(config, 'task1', 'errp', ...
	mOnlineAll);
messageAll = icmessage_serialize(mOnlineAll, sOnlineAll);
fprintf(1, 'ICMessage for MI+ErrP: %s\n', messageAll);

icmessage_delete(mOnlineAll);
icmessage_delete(mOnlineMI);
icmessage_delete(mOnlineErrP);

icserializerrapid_delete(sOnlineAll);
icserializerrapid_delete(sOnlineMI);
icserializerrapid_delete(sOnlineErrP);

ccfgtaskset_delete(tOnlineMI);
ccfgtaskset_delete(tOnlineErrP);

ccfg_delete(config);
