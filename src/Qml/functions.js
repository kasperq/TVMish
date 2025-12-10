var color_current = "dimgray"
var color_hoverAndEnabled = "gray"
var color_hoverAndDisabled = "dimgray"
var color_notHoverAndNotCurInd = "dimgray"
var color_notHoverAndCurInd = "#3f3f40"

var color_activeFocus = "skyblue"
var color_notActiveFocus = "transparent"

function setColor(curControl, curIndex, index) {    
//    if (curControl.hovered) {
    if (curControl.isHovered) {
        if (curControl.activeFocus) {
            return "lightgray";
        } else {
            if (curControl.enabled) {
                return color_hoverAndEnabled;
            } else {
                return color_hoverAndDisabled;
            }
        }
    } else {
        if (curControl.activeFocus) {
            return "lightgray";
        } else {
            if (curIndex === index) {
                return color_notHoverAndCurInd;
            } else {
                return color_notHoverAndNotCurInd;
            }
        }
    }
}

function setBorderColor(isActiveFocus) {
    return isActiveFocus ? color_activeFocus : color_notActiveFocus
}

function setScreenSize(isFull, playingUrl, tracks, position) {
    if (isFull) {
        _tvContr.showFullScreenPlayer(playingUrl, tracks, position);
//        showFullScreen();
    }
    else {
        _tvContr.closeFullScreenPlayer(playingUrl, tracks, position);
//        showNormal();
    }
}

