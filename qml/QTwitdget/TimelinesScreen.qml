import QtQuick 1.0

Item {
    property alias homeTimelineModel: homeTimelineList.model
    property alias mentionsModel: mentionsList.model
    property alias directMessagesModel: dmList.model
    property alias searchModel: searchResultList.model

    signal sendClicked(string tweetid, string text, string screenname)

    id: timelines

    TweetList {
        id: homeTimelineList
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
    }

    TweetList {
        id: mentionsList
        x: parent.width
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
        opacity: 0
    }

    TweetList {
        id: dmList
        x: 2 * parent.width
        width:  parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
        opacity: 0
    }

    SearchList {
        id: searchResultList
        x: 3 * parent.width
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
        opacity: 0
    }

    StatusUpdate {
        id: tweetUpdate
        y: - (height - topToolbar.height) //topToolbar.height
        opacity: 0
        anchors.left: parent.left
        anchors.right: parent.right

        states: State {
            name: "show"
            PropertyChanges { target: tweetUpdate;  y: topToolbar.height; opacity: 1}
        }

        transitions: [
            Transition {
                from: "";
                to: "show"
                SequentialAnimation {
                    PropertyAction { property: "opacity"; value: 1 }
                    NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack }
                }
            },
            Transition {
                from: "show"
                to: ""
                SequentialAnimation {
                    NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack }
                    PropertyAction { property: "opacity"; value: 0 }
                }
            }
        ]

        onSendButtonClicked: {
            sendClicked(tweetUpdate.tweetid, tweetUpdate.updateText, tweetUpdate.screenname);
            tweetUpdate.tweetid = "";
            tweetUpdate.screenname = "";
            tweetUpdate.updateText = "";
            tweetUpdate.state = "";
        }
    }

    Rectangle {
        id: topToolbar

        height: 82
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#939598"
            }
            GradientStop {
                position: 1
                color: "#8E9093"
            }
        }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        ButtonImage {
            id: statusUpdateButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: parent.left; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "images/statusupdate.png"
            pressedButtonImageUrl: "images/statusupdate_pressed.png"

            onClicked: {
                // ### TODO Toogle Button
                if (tweetUpdate.state == 'show')
                    tweetUpdate.state = ''
                else
                    tweetUpdate.state = 'show'
            }
        }

        ButtonImage {
            id: reloadButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: statusUpdateButton.right; anchors.leftMargin: 9
            width: 39; height: 39

            buttonImageUrl: "images/reload.png"
            pressedButtonImageUrl: "images/reload_pressed.png"
        }

        ButtonText {
            id: usernameButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: reloadButton.right; anchors.leftMargin: 10
            width: 145; height: 39

            buttonImageUrl: "images/username.png"
            pressedButtonImageUrl: "images/username_pressed.png"
            text: "@minimoog77" //test
        }

        ButtonImage {
            id: settingsButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: usernameButton.right; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "images/settings.png"
            pressedButtonImageUrl: "images/settings_pressed.png"
        }

        ButtonImage {
            id: closeButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: settingsButton.right; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "images/close.png"
            pressedButtonImageUrl: "images/close_pressed.png"

            onClicked: Qt.quit()
        }
    }

    Rectangle {
        id: bottomToolbar
        height: 96
        border.color: "#474747"
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#737477"
            }
            GradientStop {
                position: 1
                color: "#6D6E71"
            }
        }

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ButtonWithNotification {
            id: htButton
            width: 77; height: 77
            anchors.left: parent.left; anchors.leftMargin: 10
            anchors.top: parent.top; anchors.topMargin: 9
            buttonImageUrl:  "images/hometimeline.png"
            pressedButtonImageUrl: "images/hometimeline_pressed.png"
            showNotification: homeTimelineModel.numNewTweets

            onClicked: {
                homeTimelineModel.showNewTweets();
                timelines.state = "";   //default state
                mentionsButton.toggled = false;
                directMessagesButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: mentionsButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: htButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/replies.png"
            pressedButtonImageUrl: "images/replies_pressed.png"
            showNotification: mentionsModel.numNewTweets

            onClicked: {
                mentionsModel.showNewTweets();
                timelines.state = "mentions";
                htButton.toggled = false;
                directMessagesButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: directMessagesButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: mentionsButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/directmessages.png"
            pressedButtonImageUrl: "images/directmessages_pressed.png"
            //showNotification: mentionsModel.numNewTweets

            onClicked: {
                //mentionsModel.showNewTweets();
                dmList.model.showNewTweets();
                timelines.state = "directMessages"
                htButton.toggled = false;
                mentionsButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: searchButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: directMessagesButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/search.png"
            pressedButtonImageUrl: "images/search_pressed.png"
            //showNotification: mentionsModel.numNewTweets

            onClicked: {
                timelines.state = "search"
                htButton.toggled = false;
                mentionsButton.toggled = false;
                directMessagesButton.toggled = false;
            }
        }
    }

    //need some work

    states: [
        State {
            name: "mentions"
            PropertyChanges { target: mentionsList; opacity: 1 }
            PropertyChanges { target: mentionsList; x: 0 }
            PropertyChanges { target: homeTimelineList; opacity: 0 }
            PropertyChanges { target: homeTimelineList; x: - parent.width }
            PropertyChanges { target: dmList; x: parent.width }
            PropertyChanges { target: dmList; opacity: 0 }
            PropertyChanges { target: searchResultList; x: 2 * parent.width }
            PropertyChanges { target: searchResultList; opacity: 0 }
        },

        State {
            name: "directMessages"
            PropertyChanges { target: dmList; opacity: 1 }
            PropertyChanges { target: mentionsList; opacity: 0 }
            PropertyChanges { target: homeTimelineList; opacity: 0 }
            PropertyChanges { target: searchResultList; opacity: 0 }
            PropertyChanges { target: dmList; x: 0 }
            PropertyChanges { target: searchResultList; x: parent.width }
            PropertyChanges { target: mentionsList; x: - parent.width }
            PropertyChanges { target: homeTimelineList; x: - 2 * parent.width }
        },

        State {
            name: "search"
            PropertyChanges { target: searchResultList; opacity: 1 }
            PropertyChanges { target: dmList; opacity: 0 }
            PropertyChanges { target: mentionsList; opacity: 0 }
            PropertyChanges { target: homeTimelineList; opacity: 0 }
            PropertyChanges { target: homeTimelineList; x: - 3 * parent.width }
            PropertyChanges { target: mentionsList; x: - 2 * parent.width }
            PropertyChanges { target: dmList; x: - parent.width }
            PropertyChanges { target: searchResultList; x: 0 }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x, opacity"; duration: 300 }
        }
    ]
}
