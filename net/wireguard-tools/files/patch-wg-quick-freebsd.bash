--- wg-quick/freebsd.bash.orig	2023-08-11 21:45:01.833400000 +0000
+++ wg-quick/freebsd.bash	2023-08-11 21:45:19.497653000 +0000
@@ -279,26 +279,6 @@
 	ENDPOINTS=( "${added[@]}" )
 }
 
-monitor_daemon() {
-	echo "[+] Backgrounding route monitor" >&2
-	(make_temp
-	trap 'del_routes; clean_temp; exit 0' INT TERM EXIT
-	exec >/dev/null 2>&1
-	exec 19< <(exec route -n monitor)
-	local event pid=$!
-	# TODO: this should also check to see if the endpoint actually changes
-	# in response to incoming packets, and then call set_endpoint_direct_route
-	# then too. That function should be able to gracefully cleanup if the
-	# endpoints change.
-	while read -u 19 -r event; do
-		[[ $event == RTM_* ]] || continue
-		ifconfig "$INTERFACE" >/dev/null 2>&1 || break
-		[[ $AUTO_ROUTE4 -eq 1 || $AUTO_ROUTE6 -eq 1 ]] && set_endpoint_direct_route
-		# TODO: set the mtu as well, but only if up
-	done
-	kill $pid) & disown
-}
-
 HAVE_SET_DNS=0
 set_dns() {
 	[[ ${#DNS[@]} -gt 0 ]] || return 0
@@ -433,7 +413,6 @@
 		add_route "$i"
 	done
 	[[ $AUTO_ROUTE4 -eq 1 || $AUTO_ROUTE6 -eq 1 ]] && set_endpoint_direct_route
-	monitor_daemon
 	execute_hooks "${POST_UP[@]}"
 	trap 'clean_temp; exit' INT TERM EXIT
 }
