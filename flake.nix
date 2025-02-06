{
    description = "Flake to source development environment with Nix.";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/release-24.11";
    };

    outputs =
        { nixpkgs, ... }:
        let
            system = "x86_64-linux";
            pkgs = import nixpkgs { localSystem = system; };
        in
        {
            devShells.${system}.default = pkgs.mkShell {
                buildInputs = [
                    pkgs.wine64
                ];

                nativeBuildInputs = [
                    pkgs.bear
                    pkgs.pkgsCross.mingwW64.buildPackages.clang-tools
                    pkgs.pkgsCross.mingwW64.buildPackages.gcc
                ];
            };
        };
}
