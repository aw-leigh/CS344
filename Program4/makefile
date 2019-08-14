keygen: keygen.c
	gcc keygen.c -o keygen

otp: otp_enc_d otp_enc keygen

otp_enc_d: otp_enc_d.c
	gcc otp_enc_d.c -o otp_enc_d

otp_dec_d: otp_dec_d.c
	gcc otp_dec_d.c -o otp_dec_d

otp_enc: otp_enc.c
	gcc otp_enc.c -o otp_enc

otp_dec: otp_dec.c
	gcc otp_dec.c -o otp_dec

clean:
	rm -f keygen otp_enc_d otp_enc otp_dec_d otp_dec