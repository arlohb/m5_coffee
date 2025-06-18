{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages."${system}";
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang-tools_16
            platformio

            python3

            (let
              python = pkgs.python3.withPackages (ps: with ps; [ pip ]);
            in pkgs.writeShellScriptBin "fix-esptool" ''
              ${python}/bin/pip install pyserial \
                --target .pio/packages/tool-esptoolpy
            '')
          ];
        };
      }
    );
}
