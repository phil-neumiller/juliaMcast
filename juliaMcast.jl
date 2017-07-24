
function gethostname()
    hostname = Vector{UInt8}(128)
    ccall((:gethostname, "libc"), Int32,
          (Ptr{UInt8}, Csize_t),
          hostname, sizeof(hostname))
    hostname[end] = 0; # ensure null-termination
    return unsafe_string(pointer(hostname))
end

function advertise()	
	# we want to pass an advertisement count
	t1 = ccall( (:advertise, "/home/pi/juliaMcast/libJuliaMcast.so"), Int32, ());
end

function discover()	
	# we want to pass an advertisement count
	t1 = ccall( (:discover, "/home/pi/juliaMcast/libJuliaMcast.so"), Int32, ());
end

