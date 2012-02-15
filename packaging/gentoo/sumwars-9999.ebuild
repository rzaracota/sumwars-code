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

LICENSE="GPLv3 CC-BY-SA-v3"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE="stl +contenteditor"

for L in ${LANGS} ; do
	IUSE="${IUSE} linguas_${L}"
done

DEPEND="=dev-games/ogre-1.7*
=dev-games/cegui-0.7*[ogre]
dev-games/ois
dev-games/physfs
media-libs/freealut
media-libs/libvorbis
=dev-lang/lua-5.1*
contenteditor? ( dev-libs/poco )
dev-libs/tinyxml[stl=]
=net-libs/enet-1.3*"

RDEPEND="${DEPEND}"

S=${WORKDIR}/${MY_P}

src_configure() {
	strip-linguas ${LANGS}
	use stl && append-flags -DTIXML_USE_STL
	mycmakeargs=(
		-DSUMWARS_LANGUAGES=${LINGUAS}
		-DSUMWARS_NO_TINYXML=ON
		-DSUMWARS_NO_ENET=ON
		-DSUMWARS_DOC_DIR=${GAMES_DATADIR}/${PN}/doc
		-DSUMWARS_EXECUTABLE_DIR=${GAMES_BINDIR}
		-DSUMWARS_SHARE_DIR=${GAMES_DATADIR}/${PN}
		$(cmake-utils_use contenteditor SUMWARS_BUILD_TOOLS)
	)
	cmake-utils_src_configure
}

src_install() {
	cmake-utils_src_install
	newicon share/icon/SumWars.ico ${PN}.png
	make_desktop_entry ${PN} "Summoning Wars"
}
