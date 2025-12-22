{ stdenv, cmake }:

stdenv.mkDerivation {
  name = "hello_elisa";
  src = ./.;
  nativeBuildInputs = [ cmake ];

  meta = {
    mainProgram = "hello_elisa";
  };
}
