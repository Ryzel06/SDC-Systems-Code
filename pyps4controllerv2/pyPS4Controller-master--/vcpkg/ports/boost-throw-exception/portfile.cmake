# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/throw_exception
    REF boost-1.75.0
    SHA512 b4bd96f9ef37f1345db8eb26715b1da43a9f7ea2b7eaf4618be2caf3e7e4e6904fa5a32e2b62fd032dd6ebb99d0bbd7e37efbd94edb31aeb3d49c68065f3be28
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
