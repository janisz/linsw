################################################################################
#
# memopy
#
################################################################################
MEMOPY_VERSION = 1.0
MEMOPY_SITE = $(TOPDIR)/memopy 
MEMOPY_SITE_METHOD = local 
MEMOPY_DEPENDENCIES = python
MEMOPY_LICENSE = Proprietary
define MEMOPY_INSTALL_TARGET_CMDS 
   $(INSTALL) -D -m 0755 $(@D)/memopy.py $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))
