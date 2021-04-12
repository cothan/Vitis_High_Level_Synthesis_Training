buffering:
    for (auto i = 0; i < 3*N; i+= BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE II=1
        	tmp = fifo_in.read();
        	if (i < N) {
        		a_buffer[i/BUFFER][j] = tmp.data;
        	}
        	else
        	{
        		if (i < 2*N) b_buffer[(i-N)/BUFFER][j] = tmp.data;
        		else c_buffer[(i-2*N)/BUFFER][j] = tmp.data;
        	}
        }
    }
    final_sum = hls_vector_mul_part(a_buffer, b_buffer, c_buffer);
