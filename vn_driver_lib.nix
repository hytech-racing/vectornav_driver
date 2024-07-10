{ src, pkgs, stdenv , cmake, vn_driver_lib_gen_src, commslib, ... }:
stdenv.mkDerivation {
  pname = "vn_driver_lib_gen";
  version = "0.0.1";
  src = vn_driver_lib_gen_src;
  nativeBuildInputs = [ cmake ];
  propagatedBuildInputs = [ commslib ];
}