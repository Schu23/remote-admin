SET(INSTALL_DIR /etc/remoteadmin)

# Get architecture
EXECUTE_PROCESS(COMMAND dpkg --print-architecture
                OUTPUT_VARIABLE arch
                OUTPUT_STRIP_TRAILING_WHITESPACE)

SET(DATA ${PROJECT_FOLDER}/packages/debian/data)

INSTALL(TARGETS remoteadmin DESTINATION /usr/sbin)
INSTALL(DIRECTORY ${PROJECT_FOLDER}/resources DESTINATION /usr/share/remoteadmin)

INSTALL(FILES ${DATA}/vsftpd.conf          DESTINATION /usr/share/remoteadmin/data)
INSTALL(FILES ${DATA}/vsftpd               DESTINATION /usr/share/remoteadmin/data)
INSTALL(FILES ${DATA}/ftpuser.conf.default DESTINATION /usr/share/remoteadmin)

# Daemon
INSTALL(PROGRAMS ${DATA}/remoteadmin  DESTINATION /etc/init.d   PERMISSIONS OWNER_EXECUTE)

#Symlink for Wt resources
#INSTALL(DIRECTORY ${DATA}/sym/ DESTINATION /usr/share/remoteadmin/resources)


SET(CPACK_GENERATOR "DEB")
SET(CPACK_SOURCE_GENERATOR "DEB")
SET(CPACK_PACKAGE_VERSION "0.0.5")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jose Luis Lafuente")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${arch}")
SET(CPACK_DEBIAN_PACKAGE_SECTION "utils")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "bindfs, vsftpd, autofs, witty(>=3.1.10), libdb4.8++")

SET(CONTROL ${PROJECT_FOLDER}/packages/debian/control)
SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CONTROL}/postinst;${CONTROL}/conffiles;" )


INCLUDE(CPack)

 
