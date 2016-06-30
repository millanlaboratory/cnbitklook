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
	mtpath_include('$CNBITKMAT_ROOT/mextobiic');
	mtpath_include('$CNBITKMAT_ROOT/mextobiid');
end

config = ccfg_new();
if(ccfg_importfile(config, '../../extra/cnbiconfig_example.xml') == 0)
	disp('Error: cannot import file');
	exit(0);
end

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

md = idmessage_new();
mOnlineMI = icmessage_new();
mOnlineErrP = icmessage_new();
sOnlineMI = icserializerrapid_new(mOnlineMI);
sOnlineErrP = icserializerrapid_new(mOnlineErrP);

[tOnlineMI] = ccfg_online(config, 'mi', 'mi_rhlh', mOnlineMI, md);
icmessage_dumpmessage(mOnlineMI);
idmessage_dumpmessage(md);
if(tOnlineMI == 0)
	fprintf(1, 'Taskset not found\n');
	return;
end

[tOnlineErrP] = ccfg_online(config, 'smrerrp', 'errp', ...
	mOnlineErrP, 0);
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

messageMI = icmessage_serialize(sOnlineMI);
messageErrP = icmessage_serialize(sOnlineErrP);
fprintf(1, 'ICMessage for MI: %s\n', messageMI);
fprintf(1, 'ICMessage for ErrP: %s\n', messageErrP);
	
mOnlineAll = icmessage_new();
sOnlineAll = icserializerrapid_new(mOnlineAll);
[tOnlineMI2] = ccfg_online(config, 'smr', 'mi_rhlh', mOnlineAll);
[tOnlineErrP] = ccfg_online(config, 'smrerrp', 'errp', mOnlineAll);
messageAll = icmessage_serialize(sOnlineAll);
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
