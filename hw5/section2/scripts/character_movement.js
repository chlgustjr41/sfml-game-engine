// This script moves the GameObject with handle "gameobject0" (it's guid)
function move(x, y) {
    var tx = character0.x;
    var ty = character0.y;
    character0.x = tx + x;
    character0.y = ty + y;
}

// print(character0.duration);

if (character0.isMovingLeft) {
    move(-character0.duration * 10, 0);
    character0.isMovingLeft = false;
}
else if (character0.isMovingRight) {
    move(+character0.duration * 10, 0);
    character0.isMovingRight = false;
}
