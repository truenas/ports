--- source3/modules/vfs_fbsd_aio.c.orig	2019-10-17 10:26:42.389566000 -0400
+++ source3/modules/vfs_fbsd_aio.c	2019-10-18 06:53:05.680681000 -0400
@@ -0,0 +1,642 @@
+/*
+ * Use the io_uring of Linux (>= 5.1)
+ *
+ * Copyright (C) Volker Lendecke 2008
+ * Copyright (C) Jeremy Allison 2010
+ * Copyright (C) Stefan Metzmacher 2019
+ *
+ * This program is free software; you can redistribute it and/or modify
+ * it under the terms of the GNU General Public License as published by
+ * the Free Software Foundation; either version 2 of the License, or
+ * (at your option) any later version.
+ *
+ * This program is distributed in the hope that it will be useful,
+ * but WITHOUT ANY WARRANTY; without even the implied warranty of
+ * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+ * GNU General Public License for more details.
+ *
+ * You should have received a copy of the GNU General Public License
+ * along with this program; if not, write to the Free Software
+ * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
+ */
+
+#include "includes.h"
+#include "system/filesys.h"
+#include "smbd/smbd.h"
+#include "smbd/globals.h"
+#include "lib/util/tevent_unix.h"
+#include "smbprofile.h"
+#include <sys/event.h>
+#include <aio.h>
+#if 0
+#include <liburing.h>
+
+struct vfs_io_uring_request;
+
+struct vfs_io_uring_config {
+	struct io_uring uring;
+	struct tevent_fd *fde;
+	struct vfs_io_uring_request *queue;
+	struct vfs_io_uring_request *pending;
+};
+#endif
+
+struct vfs_fbsd_aio_config {
+	int kqueue_fd;
+	struct tevent_fd *fde;
+	uint nkevents;
+};
+#if 0
+struct vfs_io_uring_request {
+	struct vfs_io_uring_request *prev, *next;
+	struct tevent_req *req;
+	void *state;
+	struct io_uring_sqe sqe;
+	struct io_uring_cqe cqe;
+	struct timespec start_time;
+	struct timespec end_time;
+	SMBPROFILE_BYTES_ASYNC_STATE(profile_bytes);
+};
+#endif
+struct vfs_fbsd_aio_request {
+	struct vfs_fbsd_aio_request *prev, *next;
+	struct tevent_req *req;
+	void *state;
+	struct aiocb *iocb;
+	struct timespec start_time;
+	struct timespec end_time;
+	SMBPROFILE_BYTES_ASYNC_STATE(profile_bytes);
+};
+
+#if 0
+static int vfs_io_uring_config_destructor(struct vfs_io_uring_config *config)
+{
+	// TODO: DLIST_REMOVE loops
+	TALLOC_FREE(config->fde);
+	io_uring_queue_exit(&config->uring);
+	return 0;
+}
+#endif
+static int vfs_fbsd_aio_config_destructor(struct vfs_fbsd_aio_config *config)
+{
+	TALLOC_FREE(config->fde);
+	close(config->kqueue_fd);
+        config->kqueue_fd = -1;
+        return 0;
+}
+#if 0
+static int vfs_io_uring_request_state_deny_destructor(void *state)
+{
+	return -1;
+}
+#endif
+
+static int vfs_fbsd_aio_request_state_deny_destructor(void *state)
+{
+	return -1;
+}
+#if 0
+static void vfs_io_uring_fd_handler(struct tevent_context *ev,
+				    struct tevent_fd *fde,
+				    uint16_t flags,
+				    void *private_data);
+#endif
+static void vfs_fbsd_aio_fd_handler(struct tevent_context *ev,
+				    struct tevent_fd *fde,
+				    uint16_t flags,
+				    void *private_data);
+
+#if 0
+static int vfs_io_uring_connect(vfs_handle_struct *handle, const char *service,
+			    const char *user)
+{
+	int ret;
+	struct vfs_io_uring_config *config;
+	unsigned num_entries;
+	bool sqpoll;
+	unsigned flags = 0;
+
+	config = talloc_zero(handle->conn, struct vfs_io_uring_config);
+	if (config == NULL) {
+		DEBUG(0, ("talloc_zero() failed\n"));
+		return -1;
+	}
+
+	SMB_VFS_HANDLE_SET_DATA(handle, config,
+				NULL, struct vfs_io_uring_config,
+				return -1);
+
+	ret = SMB_VFS_NEXT_CONNECT(handle, service, user);
+	if (ret < 0) {
+		return ret;
+	}
+
+	num_entries = lp_parm_ulong(SNUM(handle->conn),
+				    "vfs_io_uring",
+				    "num_entries",
+				    128);
+	if (num_entries == 0) {
+		num_entries = 1;
+	}
+
+	sqpoll = lp_parm_bool(SNUM(handle->conn),
+			     "vfs_io_uring",
+			     "sqpoll",
+			     false);
+	if (sqpoll) {
+		flags |= IORING_SETUP_SQPOLL;
+	}
+
+	ret = io_uring_queue_init(num_entries, &config->uring, flags);
+	if (ret < 0) {
+		SMB_VFS_NEXT_DISCONNECT(handle);
+		errno = -ret;
+		return -1;
+	}
+
+	talloc_set_destructor(config, vfs_io_uring_config_destructor);
+
+	config->fde = tevent_add_fd(handle->conn->sconn->ev_ctx,
+				    config,
+				    config->uring.ring_fd,
+				    TEVENT_FD_READ,
+				    vfs_io_uring_fd_handler,
+				    handle);
+	if (config->fde == NULL) {
+		ret = errno;
+		SMB_VFS_NEXT_DISCONNECT(handle);
+		errno = ret;
+		return -1;
+	}
+
+	return 0;
+}
+#endif
+
+static int vfs_fbsd_aio_connect(vfs_handle_struct *handle, const char *service,
+				const char *user)
+{
+	int ret;
+	struct vfs_fbsd_aio_config *config;
+	bool sqpoll;
+	unsigned flags = 0;
+
+	config = talloc_zero(handle->conn, struct vfs_fbsd_aio_config);
+	if (config == NULL) {
+		DEBUG(0, ("talloc_zero() failed\n"));
+		return -1;
+	}
+
+	SMB_VFS_HANDLE_SET_DATA(handle, config,
+				NULL, struct vfs_fbsd_aio_config,
+				return -1);
+
+	ret = SMB_VFS_NEXT_CONNECT(handle, service, user);
+	if (ret < 0) {
+		return ret;
+	}
+
+	talloc_set_destructor(config, vfs_fbsd_aio_config_destructor);
+
+	config->fde = tevent_add_fd(handle->conn->sconn->ev_ctx,
+				    config,
+				    config->kqueue_fd,
+				    TEVENT_FD_READ,
+				    vfs_fbsd_aio_fd_handler,
+				    handle);
+	if (config->fde == NULL) {
+		ret = errno;
+		SMB_VFS_NEXT_DISCONNECT(handle);
+		errno = ret;
+		return -1;
+	}
+
+	return 0;
+}
+#if 0
+static void vfs_io_uring_queue_run(struct vfs_io_uring_config *config)
+{
+	struct vfs_io_uring_request *cur = NULL, *next = NULL;
+	struct io_uring_cqe *cqe = NULL;
+	unsigned cqhead;
+	unsigned nr = 0;
+	struct timespec start_time;
+	struct timespec end_time;
+	int ret;
+
+	PROFILE_TIMESTAMP(&start_time);
+
+	for (cur = config->queue; cur != NULL; cur = next) {
+		struct io_uring_sqe *sqe = NULL;
+
+		next = cur->next;
+
+		sqe = io_uring_get_sqe(&config->uring);
+		if (sqe == NULL) {
+			break;
+		}
+
+		talloc_set_destructor(cur->state,
+			vfs_io_uring_request_state_deny_destructor);
+		DLIST_REMOVE(config->queue, cur);
+		*sqe = cur->sqe;
+		DLIST_ADD_END(config->pending, cur);
+		SMBPROFILE_BYTES_ASYNC_SET_BUSY(cur->profile_bytes);
+
+		cur->start_time = start_time;
+	}
+
+	ret = io_uring_submit(&config->uring);
+	if (ret < 0) {
+		// TODO
+	}
+
+	PROFILE_TIMESTAMP(&end_time);
+
+	io_uring_for_each_cqe(&config->uring, cqhead, cqe) {
+		struct tevent_req *req = NULL;
+
+		cur = (struct vfs_io_uring_request *)(uintptr_t)cqe->user_data;
+		req = talloc_get_type_abort(cur->req, struct tevent_req);
+
+		talloc_set_destructor(cur->state, NULL);
+		DLIST_REMOVE(config->pending, cur);
+		cur->cqe = *cqe;
+
+		SMBPROFILE_BYTES_ASYNC_SET_IDLE(cur->profile_bytes);
+		cur->end_time = end_time;
+
+		/*
+		 * We rely on tevent_req_defer_callback() being called
+		 * already.
+		 */
+		tevent_req_done(req);
+		nr++;
+	}
+
+	io_uring_cq_advance(&config->uring, nr);
+}
+#endif
+static void vfs_fbsd_aio_queue_run(struct vfs_fbsd_aio_config *config)
+{
+	int ret, i;
+	struct timespec timeout = {0,0};
+	struct kevent received[config->nkevents];
+	struct vfs_fbsd_aio_request *cur = NULL;
+
+	ret = kevent(config->kqueue_fd,
+		     NULL,		/* changelist */
+		     0,			/* nchanges */
+		     received,		/* eventlist */
+		     config->nkevents,	/* nevents */
+		     &timeout);		/* timeout */
+
+	for (i=0;i<ret;i++) {
+		struct tevent_req *req = NULL;
+		if (received[i].filter != EVFILT_AIO) {
+			DBG_ERR("Had something that wasn't AIO in queue\n");
+		}
+		cur = (struct vfs_fbsd_aio_request *)(uintptr_t)received[i].udata;
+		req = talloc_get_type_abort(cur->req, struct tevent_req);
+
+		//talloc_set_destructor(cur->state, NULL);
+		config->nkevents--;
+		tevent_req_done(req);
+	}
+}
+#if 0
+static void vfs_io_uring_fd_handler(struct tevent_context *ev,
+				    struct tevent_fd *fde,
+				    uint16_t flags,
+				    void *private_data)
+{
+	vfs_handle_struct *handle = (vfs_handle_struct *)private_data;
+	struct vfs_io_uring_config *config = NULL;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config,
+				struct vfs_io_uring_config,
+				smb_panic(__location__));
+
+	vfs_io_uring_queue_run(config);
+}
+#endif 
+
+static void vfs_fbsd_aio_fd_handler(struct tevent_context *ev,
+				    struct tevent_fd *fde,
+				    uint16_t flags,
+				    void *private_data)
+{
+	vfs_handle_struct *handle = (vfs_handle_struct *)private_data;
+	struct vfs_fbsd_aio_config *config = NULL;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config,
+				struct vfs_fbsd_aio_config,
+				smb_panic(__location__));
+
+	vfs_fbsd_aio_queue_run(config);
+}
+#if 0
+struct vfs_io_uring_pread_state {
+	struct vfs_io_uring_request ur;
+	struct iovec iov;
+};
+#endif
+
+struct vfs_fbsd_aio_pread_state {
+	struct vfs_fbsd_aio_request fbio;
+	struct iovec iov;
+};
+
+static struct tevent_req *vfs_fbsd_aio_pread_send(struct vfs_handle_struct *handle,
+					     TALLOC_CTX *mem_ctx,
+					     struct tevent_context *ev,
+					     struct files_struct *fsp,
+					     void *data,
+					     size_t n, off_t offset)
+{
+	int ret;
+	DBG_ERR("entered pread_send\n");
+	struct tevent_req *req = NULL;
+	struct vfs_fbsd_aio_pread_state *state = NULL;
+	struct vfs_fbsd_aio_config *config = NULL;
+	struct aiocb *kq_iocb;
+	//bzero(kq_iocb, sizeof(*kq_iocb));
+	ZERO_STRUCT(kq_iocb);
+	DBG_ERR("bzeroed the mofo\n");
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config,
+				struct vfs_fbsd_aio_config,
+				smb_panic(__location__));
+
+	req = tevent_req_create(mem_ctx, &state,
+				struct vfs_fbsd_aio_pread_state);
+	if (req == NULL) {
+		return NULL;
+	}
+	if (n == NULL) {
+		DBG_ERR('n is null\n');
+	}
+	DBG_ERR("fd: [%d], nd: %d\n", fsp->fh->fd, n);
+	state->fbio.state = state;
+	state->iov.iov_base = (void *)data;
+	//state->iov.iov_len = n;
+	DBG_ERR("got here\n");
+	state->fbio.req = req;
+	//state->fbio.iocb = kq_iocb;
+	DBG_ERR("got here3\n");
+	kq_iocb->aio_nbytes = n;
+	DBG_ERR("got here\n");
+	kq_iocb->aio_buf = state->iov.iov_base;
+	DBG_ERR("got here\n");
+	kq_iocb->aio_fildes = fsp->fh->fd;
+	kq_iocb->aio_offset = offset;
+	DBG_ERR("got here\n");
+	kq_iocb->aio_sigevent.sigev_notify_kqueue = config->kqueue_fd;
+	kq_iocb->aio_sigevent.sigev_value.sival_ptr = &state; //this is gettings stupid
+	kq_iocb->aio_sigevent.sigev_notify = SIGEV_KEVENT;
+	DBG_ERR("before aio_read\n");
+	ret = aio_read(kq_iocb);
+	if (ret != 0) {
+		DBG_ERR("Failed to add to AIO queue %s\n", strerror(errno));
+		return NULL; //errno to status
+	}
+	DBG_ERR("Got past aio_read\n");
+	DBG_ERR("state iocb fd: %d, kq_iocb fd: %d\n", state->fbio.iocb->aio_fildes, kq_iocb->aio_fildes);
+	config->nkevents++;
+	vfs_fbsd_aio_queue_run(config);
+
+	if (!tevent_req_is_in_progress(req)) {
+		return tevent_req_post(req, ev);
+	}
+
+	tevent_req_defer_callback(req, ev);
+	return req;
+}
+#if 0
+static ssize_t vfs_io_uring_pread_recv(struct tevent_req *req,
+				  struct vfs_aio_state *vfs_aio_state)
+{
+	struct vfs_io_uring_pread_state *state = tevent_req_data(
+		req, struct vfs_io_uring_pread_state);
+	int ret;
+
+	vfs_aio_state->duration = nsec_time_diff(&state->ur.end_time,
+						 &state->ur.start_time);
+
+	if (tevent_req_is_unix_error(req, &vfs_aio_state->error)) {
+		return -1;
+	}
+
+	if (state->ur.cqe.res < 0) {
+		vfs_aio_state->error = -state->ur.cqe.res;
+		ret = -1;
+	} else {
+		vfs_aio_state->error = 0;
+		ret = state->ur.cqe.res;
+	}
+
+	tevent_req_received(req);
+	return ret;
+}
+#endif
+
+static ssize_t vfs_fbsd_aio_pread_recv(struct tevent_req *req,
+				  struct vfs_aio_state *vfs_aio_state)
+{
+	struct vfs_fbsd_aio_pread_state *state = tevent_req_data(
+		req, struct vfs_fbsd_aio_pread_state);
+	int ret;
+	ret = aio_return(&state->fbio.iocb);
+	if (ret != 0) {
+		DBG_ERR("aio_return failed: %s\n", strerror(errno));
+		vfs_aio_state->error = ret;
+		return -1;
+	}
+#if 0
+	vfs_aio_state->duration = nsec_time_diff(&state->ur.end_time,
+						 &state->ur.start_time);
+#endif
+	if (tevent_req_is_unix_error(req, &vfs_aio_state->error)) {
+		DBG_ERR("Is unix error\n");
+		return -1;
+	}
+
+	tevent_req_received(req);
+	return ret;
+}
+#if 0
+struct vfs_io_uring_pwrite_state {
+	struct vfs_io_uring_request ur;
+	struct iovec iov;
+};
+
+static struct tevent_req *vfs_io_uring_pwrite_send(struct vfs_handle_struct *handle,
+					      TALLOC_CTX *mem_ctx,
+					      struct tevent_context *ev,
+					      struct files_struct *fsp,
+					      const void *data,
+					      size_t n, off_t offset)
+{
+	struct tevent_req *req = NULL;
+	struct vfs_io_uring_pwrite_state *state = NULL;
+	struct vfs_io_uring_config *config = NULL;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config,
+				struct vfs_io_uring_config,
+				smb_panic(__location__));
+
+	req = tevent_req_create(mem_ctx, &state,
+				struct vfs_io_uring_pwrite_state);
+	if (req == NULL) {
+		return NULL;
+	}
+	state->ur.req = req;
+	state->ur.state = state;
+
+	SMBPROFILE_BYTES_ASYNC_START(syscall_asys_fsync, profile_p,
+				     state->ur.profile_bytes, 0);
+	SMBPROFILE_BYTES_ASYNC_SET_IDLE(state->ur.profile_bytes);
+
+	state->iov.iov_base = discard_const(data);
+	state->iov.iov_len = n;
+	state->ur.sqe = (struct io_uring_sqe) {
+		.user_data = (uintptr_t)&state->ur,
+		.opcode = IORING_OP_WRITEV,
+		.flags = 0,
+		.ioprio = 0,
+		.fd = fsp->fh->fd,
+		.off = offset,
+		.addr = (uintptr_t)&state->iov,
+		.len = 1,
+		.rw_flags = 0,
+	};
+	DLIST_ADD_END(config->queue, &state->ur);
+
+	vfs_io_uring_queue_run(config);
+
+	if (!tevent_req_is_in_progress(req)) {
+		return tevent_req_post(req, ev);
+	}
+
+	tevent_req_defer_callback(req, ev);
+	return req;
+}
+
+static ssize_t vfs_io_uring_pwrite_recv(struct tevent_req *req,
+				   struct vfs_aio_state *vfs_aio_state)
+{
+	struct vfs_io_uring_pwrite_state *state = tevent_req_data(
+		req, struct vfs_io_uring_pwrite_state);
+	int ret;
+
+	vfs_aio_state->duration = nsec_time_diff(&state->ur.end_time,
+						 &state->ur.start_time);
+
+	if (tevent_req_is_unix_error(req, &vfs_aio_state->error)) {
+		return -1;
+	}
+
+	if (state->ur.cqe.res < 0) {
+		vfs_aio_state->error = -state->ur.cqe.res;
+		ret = -1;
+	} else {
+		vfs_aio_state->error = 0;
+		ret = state->ur.cqe.res;
+	}
+
+	tevent_req_received(req);
+	return ret;
+}
+
+struct vfs_io_uring_fsync_state {
+	struct vfs_io_uring_request ur;
+};
+
+static struct tevent_req *vfs_io_uring_fsync_send(struct vfs_handle_struct *handle,
+					     TALLOC_CTX *mem_ctx,
+					     struct tevent_context *ev,
+					     struct files_struct *fsp)
+{
+	struct tevent_req *req = NULL;
+	struct vfs_io_uring_fsync_state *state = NULL;
+	struct vfs_io_uring_config *config = NULL;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config,
+				struct vfs_io_uring_config,
+				smb_panic(__location__));
+
+	req = tevent_req_create(mem_ctx, &state,
+				struct vfs_io_uring_fsync_state);
+	if (req == NULL) {
+		return NULL;
+	}
+	state->ur.req = req;
+	state->ur.state = state;
+
+	SMBPROFILE_BYTES_ASYNC_START(syscall_asys_fsync, profile_p,
+				     state->ur.profile_bytes, 0);
+	SMBPROFILE_BYTES_ASYNC_SET_IDLE(state->ur.profile_bytes);
+
+	state->ur.sqe = (struct io_uring_sqe) {
+		.user_data = (uintptr_t)&state->ur,
+		.opcode = IORING_OP_FSYNC,
+		.flags = 0,
+		.ioprio = 0,
+		.fd = fsp->fh->fd,
+		.fsync_flags = 0,
+	};
+	DLIST_ADD_END(config->queue, &state->ur);
+
+	vfs_io_uring_queue_run(config);
+
+	if (!tevent_req_is_in_progress(req)) {
+		return tevent_req_post(req, ev);
+	}
+
+	tevent_req_defer_callback(req, ev);
+	return req;
+}
+
+static int vfs_io_uring_fsync_recv(struct tevent_req *req,
+			      struct vfs_aio_state *vfs_aio_state)
+{
+	struct vfs_io_uring_fsync_state *state = tevent_req_data(
+		req, struct vfs_io_uring_fsync_state);
+	int ret;
+
+	vfs_aio_state->duration = nsec_time_diff(&state->ur.end_time,
+						 &state->ur.start_time);
+
+	if (tevent_req_is_unix_error(req, &vfs_aio_state->error)) {
+		return -1;
+	}
+
+	if (state->ur.cqe.res < 0) {
+		vfs_aio_state->error = -state->ur.cqe.res;
+		ret = -1;
+	} else {
+		vfs_aio_state->error = 0;
+		ret = state->ur.cqe.res;
+	}
+
+	tevent_req_received(req);
+	return ret;
+}
+#endif
+
+static struct vfs_fn_pointers vfs_fbsd_aio_fns = {
+	.connect_fn = vfs_fbsd_aio_connect,
+	.pread_send_fn = vfs_fbsd_aio_pread_send,
+	.pread_recv_fn = vfs_fbsd_aio_pread_recv,
+	//.pwrite_send_fn = vfs_io_uring_pwrite_send,
+	//.pwrite_recv_fn = vfs_io_uring_pwrite_recv,
+	//.fsync_send_fn = vfs_io_uring_fsync_send,
+	//.fsync_recv_fn = vfs_io_uring_fsync_recv,
+};
+
+static_decl_vfs;
+NTSTATUS vfs_fbsd_aio_init(TALLOC_CTX *ctx)
+{
+	return smb_register_vfs(SMB_VFS_INTERFACE_VERSION,
+				"fbsd_aio", &vfs_fbsd_aio_fns);
+}
