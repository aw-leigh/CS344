keygen: keygen.c
	gcc keygen.c -o keygen

reset:
	make otp && killall -15 otp_enc_d && killall -15 otp_dec_d && clear

otp: otp_enc_d otp_enc otp_dec_d otp_dec keygen

otp_enc_d: otp_enc_d.c
	gcc  -std=c99 otp_enc_d.c -o otp_enc_d

otp_dec_d: otp_dec_d.c
	gcc  -std=c99 otp_dec_d.c -o otp_dec_d

otp_enc: otp_enc.c
	gcc  -std=c99 otp_enc.c -o otp_enc

otp_dec: otp_dec.c
	gcc  -std=c99 otp_dec.c -o otp_dec

clean:
	rm -f keygen otp_enc_d otp_enc otp_dec_d otp_dec