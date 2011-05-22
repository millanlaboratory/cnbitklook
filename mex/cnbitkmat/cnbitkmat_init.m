[~, basepath] = mtpath_basename(which('cnbitkmat_init.m'));

mtpath_include([basepath '/matndf/']);
mtpath_include([basepath '/mexcnbiloop']);
mtpath_include([basepath '/mexcnbiconfig']);
mtpath_include([basepath '/mextransport']);
mtpath_include([basepath '/mextobicore']);
mtpath_include([basepath '/mextobiic']);
mtpath_include([basepath '/mextobiid']);
