INSTALL(DIRECTORY ./ DESTINATION bin
  PATTERN "CMakeLists.txt" EXCLUDE
  PATTERN "./*.sh" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
)
