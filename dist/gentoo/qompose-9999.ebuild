EAPI=5
inherit git-2

DESCRIPTION="A simple programmer's text editor."
EGIT_REPO_URI="https://github.com/CmdrMoozy/qompose.git"
HOMEPAGE="https://github.com/CmdrMoozy/qompose"
KEYWORDS="~x86 ~amd64"
SLOT="0"
LICENSE="GPL-3"
IUSE=""

RDEPEND=">=dev-util/cmake-2.8.9
	>=dev-libs/icu-50.0
	dev-qt/qtcore:5
	dev-qt/qtgui:5
	dev-qt/qtwidgets:5
	dev-qt/qtnetwork:5
	dev-qt/qtprintsupport:5"
DEPEND="${RDEPEND}"

src_compile() {
	
	${S}/release.sh || die
	
}

src_install() {
	
	exeinto /usr/bin
	
	doexe build/Qompose
	
}
