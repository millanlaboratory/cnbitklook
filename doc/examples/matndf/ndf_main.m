% 2010-12-09  Michele Tavella <michele.tavella@epfl.ch>
function ndf_main(plot, psd)
if(nargin == 0)
	plot = 0;
	psd = 0;
end

ndf_example_ack('/tmp/ndf.example.ack.0', '127.0.0.1', '8000', 0, 0);
