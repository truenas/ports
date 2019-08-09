--- plugins.d/alarm-notify.sh.orig	2019-08-09 03:06:46 UTC
+++ plugins.d/alarm-notify.sh
@@ -153,7 +153,7 @@ custom_sender() {
 # -----------------------------------------------------------------------------
 # defaults to allow running this script by hand

-[ -z "${NETDATA_CONFIG_DIR}"   ] && NETDATA_CONFIG_DIR="$(dirname "${0}")/../../../../etc/netdata"
+[ -z "${NETDATA_CONFIG_DIR}"   ] && NETDATA_CONFIG_DIR="$(dirname "${0}")/../../../etc/netdata"
 [ -z "${NETDATA_CACHE_DIR}"    ] && NETDATA_CACHE_DIR="$(dirname "${0}")/../../../../var/cache/netdata"
 [ -z "${NETDATA_REGISTRY_URL}" ] && NETDATA_REGISTRY_URL="https://registry.my-netdata.io"
