
function gethostname()
    hostname = Vector{UInt8}(128)
    ccall((:gethostname, "libc"), Int32,
          (Ptr{UInt8}, Csize_t),
          hostname, sizeof(hostname))
    hostname[end] = 0; # ensure null-termination
    return unsafe_string(pointer(hostname))
end

function testLib()	
	# we want to pass an advertisement count
	t1 = ccall( (:testLib, "/home/pi/juliaMcast/libJuliaMcast.so"), Int32, ());
end

