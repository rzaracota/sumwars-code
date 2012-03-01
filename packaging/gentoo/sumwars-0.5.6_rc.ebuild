# Copyright 1999-2012 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $ Exp $

EAPI=3

SCM=""
if [ "${PV}" == "9999" ] ;
then
	SCM=mercurial
	EHG_REPO_URI="https://bitbucket.org/sumwars/sumwars-code"
	EHG_REVISION="default"
	EHG_PROJECT="${PN}"

	SRC_URI=""
	MY_P=${PN}
else
	SRC_URI="mirror://sourceforge/sumwars/${P//_/-}-src.tar.bz2"
	MY_P=${PN}-$(expr "${PV}" : '\([0-9.]*\)')
fi

inherit games cmake-utils flag-o-matic eutils ${SCM}

DESCRIPTION="a multi-player, 3D action role-playing game"
HOMEPAGE="http://sumwars.org"

LANGS="de en it pl pt ru uk"

LICENSE="GPL-3 CCPL-Attribution-ShareAlike-3.0"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE="+stl +tools debug"

for L in ${LANGS} ; do
	IUSE="${IUSE} linguas_${L}"
done

DEPEND="  =dev-games/cegui-0.7*[ogre]
         >=dev-games/ogre-1.7
        !>=dev-games/ogre-1.9
           dev-games/ois
           dev-games/physfs
          =dev-lang/lua-5.1*
           dev-libs/tinyxml[stl=]
           media-libs/freealut
           media-libs/openal
           media-libs/libogg
           media-libs/libvorbis
          =net-libs/enet-1.3*
           x11-libs/libXrandr
  tools? ( dev-libs/poco )"

RDEPEND="${DEPEND}"

S=${WORKDIR}/${MY_P}

src_configure() {
	strip-linguas ${LANGS}
	use stl && append-flags -DTIXML_USE_STL
	use debug && CMAKE_BUILD_TYPE=Debug

	# configure sumwars with cmake
	mycmakeargs=(
		-DCMAKE_INSTALL_PREFIX="/usr"
		-DSUMWARS_LANGUAGES="${LINGUAS}"
		-DSUMWARS_NO_TINYXML=ON
		-DSUMWARS_NO_ENET=ON
		-DSUMWARS_DOC_DIR="share/doc/${PF}"
		-DSUMWARS_EXECUTABLE_DIR="${GAMES_BINDIR#/usr/}"
		-DSUMWARS_SHARE_DIR="${GAMES_DATADIR#/usr/}/${PN}"
		-DSUMWARS_STANDALONE_MODE=OFF
		-DSUMWARS_POST_BUILD_COPY=OFF
		-DSUMWARS_PORTABLE_MODE=OFF
		-DSUMWARS_RANDOM_REGIONS=ON
		$(cmake-utils_use tools SUMWARS_BUILD_TOOLS)
	)

	cmake-utils_src_configure
}

src_install() {
	cmake-utils_src_install
	prepgamesdirs
	newicon share/icon/SumWars.ico ${PN}.ico
	make_desktop_entry ${PN} "Summoning Wars" ${PN}.ico
}
