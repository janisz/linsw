################################################################################
#
# memo
#
################################################################################

MEMO_VERSION = 1.0
MEMO_SITE = $(TOPDIR)/memo
MEMO_SITE_METHOD = local

define MEMO_BUILD_CMDS
   $(MAKE) $(TARGET_CONFIGURE_OPTS) memo -C $(@D)
endef
define MEMO_INSTALL_TARGET_CMDS 
   $(INSTALL) -D -m 0755 $(@D)/memo $(TARGET_DIR)/usr/bin 
endef
MEMO_LICENSE = Proprietary

$(eval $(generic-package))
