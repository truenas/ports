--- ./source3/smbd/smb2_ioctl_network_fs.c.orig	2014-02-22 10:46:04.901338821 -0800
+++ ./source3/smbd/smb2_ioctl_network_fs.c	2014-02-22 10:46:43.331970910 -0800
@@ -394,8 +394,8 @@
 
 	SIVAL(out_output->data, 0x00, conn->smb2.server.capabilities);
 	memcpy(out_output->data+0x04, out_guid_blob.data, 16);
-	SIVAL(out_output->data, 0x14, conn->smb2.server.security_mode);
-	SIVAL(out_output->data, 0x16, conn->smb2.server.dialect);
+	SSVAL(out_output->data, 0x14, conn->smb2.server.security_mode);
+	SSVAL(out_output->data, 0x16, conn->smb2.server.dialect);
 
 	return NT_STATUS_OK;
 }
