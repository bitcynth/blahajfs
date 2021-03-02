{ pkgs ? import <nixpkgs> {} }:

let

  inherit (pkgs) stdenv lib;

in stdenv.mkDerivation {
  pname = "blahajfs";
  version = "1337";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    pkgs.pkg-config
  ];

  buildInputs = [
    pkgs.fuse
  ];

  NIX_CFLAGS_COMPILE = "-D_FILE_OFFSET_BITS=64";

  installPhase = ''
    runHook preinstall
    mkdir -p $out/bin
    install -D -m 755 blahajfs $out/bin/blahajfs
    runHook postInstall
  '';

}
