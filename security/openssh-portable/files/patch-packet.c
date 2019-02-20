--- packet.c.orig	2018-10-16 17:01:20.000000000 -0700
+++ packet.c	2018-11-10 11:35:07.816193000 -0800
@@ -2253,6 +2253,8 @@ int
 ssh_packet_get_state(struct ssh *ssh, struct sshbuf *m)
 {
 	struct session_state *state = ssh->state;
+	u_char *p;
+	size_t slen, rlen;
 	int r;
 
 	if ((r = kex_to_blob(m, ssh->kex)) != 0 ||
@@ -2272,6 +2274,22 @@ ssh_packet_get_state(struct ssh *ssh, struct sshbuf *m)
 	    (r = sshbuf_put_stringb(m, state->output)) != 0)
 		return r;
 
+	slen = cipher_get_keycontext(state->send_context, NULL);
+	rlen = cipher_get_keycontext(state->receive_context, NULL);
+	if ((r = sshbuf_put_u32(m, slen)) != 0 ||
+	    (r = sshbuf_reserve(m, slen, &p)) != 0)
+		return r;
+	if (cipher_get_keycontext(state->send_context, p) != (int)slen)
+		return SSH_ERR_INTERNAL_ERROR;
+	if ((r = sshbuf_put_u32(m, rlen)) != 0 ||
+	    (r = sshbuf_reserve(m, rlen, &p)) != 0)
+		return r;
+	if (cipher_get_keycontext(state->receive_context, p) != (int)rlen)
+		return SSH_ERR_INTERNAL_ERROR;
+	if ((r = sshbuf_put_stringb(m, state->input)) != 0 ||
+	    (r = sshbuf_put_stringb(m, state->output)) != 0)
+		return r;
+
 	return 0;
 }
 
@@ -2386,8 +2404,8 @@ int
 ssh_packet_set_state(struct ssh *ssh, struct sshbuf *m)
 {
 	struct session_state *state = ssh->state;
-	const u_char *input, *output;
-	size_t ilen, olen;
+	const u_char *keyin, *keyout, *input, *output;
+	size_t rlen, slen, ilen, olen;
 	int r;
 
 	if ((r = kex_from_blob(m, &ssh->kex)) != 0 ||
@@ -2414,6 +2432,15 @@ ssh_packet_set_state(struct ssh *ssh, struct sshbuf *m)
 	    (r = ssh_set_newkeys(ssh, MODE_OUT)) != 0)
 		return r;
 
+	if ((r = sshbuf_get_string_direct(m, &keyout, &slen)) != 0 ||
+	    (r = sshbuf_get_string_direct(m, &keyin, &rlen)) != 0)
+		return r;
+	if (cipher_get_keycontext(state->send_context, NULL) != (int)slen ||
+	    cipher_get_keycontext(state->receive_context, NULL) != (int)rlen)
+		return SSH_ERR_INVALID_FORMAT;
+	cipher_set_keycontext(state->send_context, keyout);
+	cipher_set_keycontext(state->receive_context, keyin);
+
 	if ((r = ssh_packet_set_postauth(ssh)) != 0)
 		return r;
