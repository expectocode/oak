"""
cargo-raze crate build file.

DO NOT EDIT! Replaced on runs of cargo-raze
"""
package(default_visibility = [
  # Public for visibility by "@raze__crate__version//" targets.
  #
  # Prefer access through "//cargo", which limits external
  # visibility to explicit Cargo.toml dependencies.
  "//visibility:public",
])

licenses([
  "notice", # "MIT"
])

load(
    "@io_bazel_rules_rust//rust:rust.bzl",
    "rust_library",
    "rust_binary",
    "rust_test",
)


# Unsupported target "concurrency" with type "test" omitted
# Unsupported target "rate" with type "test" omitted

rust_library(
    name = "tower_limit",
    crate_root = "src/lib.rs",
    crate_type = "lib",
    edition = "2018",
    srcs = glob(["**/*.rs"]),
    deps = [
        "@raze__futures_core__0_3_4//:futures_core",
        "@raze__pin_project__0_4_9//:pin_project",
        "@raze__tokio__0_2_19//:tokio",
        "@raze__tower_layer__0_3_0//:tower_layer",
        "@raze__tower_load__0_3_0//:tower_load",
        "@raze__tower_service__0_3_0//:tower_service",
    ],
    rustc_flags = [
        "--cap-lints=allow",
    ],
    version = "0.3.1",
    crate_features = [
    ],
)

