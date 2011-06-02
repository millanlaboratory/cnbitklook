% 2011-06-02  Michele Tavella <michele.tavella@epfl.ch>

i = tid_new();
m = idmessage_new();
s = idserializerrapid_new(m);
m2 = idmessage_new();
s2 = idserializerrapid_new(m2);

idmessage_setdescription(m, 'idtest');
idmessage_setfamilytype(m, idmessage_familytype('biosig'));

tid_attach(i);

event = 1;
while(tid_isattached(i) == true)
	idmessage_setevent(m, event);
	tid_setmessage(i, s, event);
	event = event + 1;
	disp('Checking messages:');
	while(tid_getmessage(i, s2))
		idmessage_dumpmessage(m2);
	end
	if(event == 100)
		break;
	end
	pause(1);
end
tid_detach(i);
i = tid_delete(i);
