{
    description = "Flake to source development environment with Nix.";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    };

    outputs =
        { nixpkgs, ... }:
        let
            system = "x86_64-linux";
            pkgs = import nixpkgs { localSystem = system; };
        in
        {
            devShells.${system}.default =
                with pkgs;
                mkShell.override { stdenv = pkgsCross.mingwW64.stdenv; } {
                    nativeBuildInputs = [
                        wine64
                        bear
                    ];
                };
        };
}
