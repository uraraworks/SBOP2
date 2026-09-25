# SBOP2 ステージング用イメージ(SboSvr + ブラウザ版)。
# docs/cloud-dev-staging-plan.md の S6。ビルドコンテキストはリポジトリのルート(.dockerignore 参照)。
#
# ブラウザ版は事前に out/browser-title へビルドし、wss シムを入れておくこと
# (deploy/staging/prepare-browser.sh)。wasm は CPU に依存しないので、
# x86 の CI で作ったものを arm64 のホストでもそのまま使える。
# SboSvr はこのイメージの中でビルドするので、ホストの CPU(x86_64 / arm64)を問わない。

FROM ubuntu:24.04 AS build
RUN apt-get update \
	&& apt-get install -y --no-install-recommends cmake g++ make zlib1g-dev \
	&& rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY CMakeLists.txt ./
COPY cmake ./cmake
COPY Common ./Common
COPY myLib ./myLib
COPY SboSockLib ./SboSockLib
COPY SboSvr ./SboSvr
COPY SboSvrTest ./SboSvrTest
# ステージングは _DEBUG なし(SBO_DEBUG_API は既定の OFF のまま)
RUN cmake -S . -B /build -DCMAKE_BUILD_TYPE=Release \
	&& cmake --build /build -j"$(nproc)" --target SboSvr

FROM ubuntu:24.04
RUN apt-get update \
	&& apt-get install -y --no-install-recommends zlib1g ca-certificates \
	&& rm -rf /var/lib/apt/lists/* \
	&& useradd --system --home-dir /srv/sbop2 --shell /usr/sbin/nologin sbo
WORKDIR /srv/sbop2
# 実行ファイルの隣に webroot/(game にブラウザ版)と SBODATA/ を置く(Linux 版の配置と同じ)
COPY --from=build /build/SboSvr ./SboSvr
COPY SboSvr/webroot ./webroot
COPY out/browser-title/ ./webroot/game/
COPY Release/SBODATA/ ./SBODATA/
RUN chown -R sbo:sbo /srv/sbop2
USER sbo
# DB(SboData.db)・pid・ログは SBODATA/ に書かれるので、ここをボリュームにする。
# 名前付きボリュームは初回作成時にイメージ内の .dat がコピーされる。
VOLUME ["/srv/sbop2/SBODATA"]
# SIGTERM を受けると DB を保存して終わる(docker stop で安全に止まる)
STOPSIGNAL SIGTERM
CMD ["./SboSvr", "--headless"]
