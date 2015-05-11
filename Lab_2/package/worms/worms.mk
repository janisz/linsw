################################################################################
#
# worms
#
################################################################################

WORMS_VERSION = 1.0
WORMS_SITE = $(TOPDIR)/worms_src
WORMS_SITE_METHOD = local
WORMS_DEPENDENCIES = ncurses

define WORMS_BUILD_CMDS
   $(MAKE) $(TARGET_CONFIGURE_OPTS) worms -C $(@D)
endef
define WORMS_INSTALL_TARGET_CMDS 
   $(INSTALL) -D -m 0755 $(@D)/worms $(TARGET_DIR)/usr/bin 
endef
WORMS_LICENSE = Proprietary

$(eval $(generic-package))
