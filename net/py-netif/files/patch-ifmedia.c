--- ifmedia.c.orig	2017-12-26 12:30:35 UTC
+++ ifmedia.c
@@ -49,15 +49,6 @@ static struct ifmedia_description ifm_subtype_ethernet
 static struct ifmedia_description ifm_subtype_ethernet_option_descriptions[] =
     IFM_SUBTYPE_ETHERNET_OPTION_DESCRIPTIONS;
 
-static struct ifmedia_description ifm_subtype_tokenring_descriptions[] =
-    IFM_SUBTYPE_TOKENRING_DESCRIPTIONS;
-
-static struct ifmedia_description ifm_subtype_tokenring_aliases[] =
-    IFM_SUBTYPE_TOKENRING_ALIASES;
-
-static struct ifmedia_description ifm_subtype_tokenring_option_descriptions[] =
-    IFM_SUBTYPE_TOKENRING_OPTION_DESCRIPTIONS;
-
 static struct ifmedia_description ifm_subtype_fddi_descriptions[] =
     IFM_SUBTYPE_FDDI_DESCRIPTIONS;
 
@@ -109,32 +100,11 @@ static struct ifmedia_type_to_subtype ifmedia_types_to
 		{
 			{ &ifm_subtype_shared_descriptions[0], 0 },
 			{ &ifm_subtype_shared_aliases[0], 1 },
-			{ &ifm_subtype_ethernet_descriptions[0], 0 },
-			{ &ifm_subtype_ethernet_aliases[0], 1 },
 			{ NULL, 0 },
 		},
 		{
 			{ &ifm_shared_option_descriptions[0], 0 },
 			{ &ifm_shared_option_aliases[0], 1 },
-			{ &ifm_subtype_ethernet_option_descriptions[0], 0 },
-			{ NULL, 0 },
-		},
-		{
-			{ NULL, 0 },
-		},
-	},
-	{
-		{
-			{ &ifm_subtype_shared_descriptions[0], 0 },
-			{ &ifm_subtype_shared_aliases[0], 1 },
-			{ &ifm_subtype_tokenring_descriptions[0], 0 },
-			{ &ifm_subtype_tokenring_aliases[0], 1 },
-			{ NULL, 0 },
-		},
-		{
-			{ &ifm_shared_option_descriptions[0], 0 },
-			{ &ifm_shared_option_aliases[0], 1 },
-			{ &ifm_subtype_tokenring_option_descriptions[0], 0 },
 			{ NULL, 0 },
 		},
 		{
