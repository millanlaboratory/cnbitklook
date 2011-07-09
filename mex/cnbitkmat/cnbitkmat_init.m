[~, basepath] = mtpath_basename(which('cnbitkmat_init.m'));
	
disp(['[cnbitkmat_init] $CNBITKMAT_ROOT: ' getenv('CNBITKMAT_ROOT')]);
disp(['[cnbitkmat_init] $TOBICOREMAT_ROOT: ' getenv('TOBICOREMAT_ROOT')]);

mtpath_include([basepath '/matndf/']);
mtpath_include([basepath '/mexcnbiloop']);
mtpath_include([basepath '/mexcnbiconfig']);
mtpath_include([basepath '/mextransport']);

if(isempty(getenv('TOBICOREMAT_ROOT')))
    setenv('TOBICOREMAT_ROOT', '/usr/share/tobicore/tobicoremat/');
end
mtpath_include('$TOBICOREMAT_ROOT');
