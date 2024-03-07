--- kmip/demos/pie/locate.py.orig	2023-04-28 13:23:19 UTC
+++ kmip/demos/pie/locate.py
@@ -63,7 +63,7 @@ if __name__ == '__main__':
     for initial_date in initial_dates:
         try:
             t = time.strptime(initial_date)
-        except ValueError, TypeError:
+        except (ValueError, TypeError):
             logger.error(
                 "Invalid initial date provided: {}".format(initial_date)
             )
