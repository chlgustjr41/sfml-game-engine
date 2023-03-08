// This script moves the GameObject with handle "gameobject0" (it's guid)
function move(handle, x, y) {
    var tx = handle.x;
    var ty = handle.y;
    handle.x = tx + x;
    handle.y = ty + y;
}

// print(character0.duration);
for (var i = 0; i < getProjectileCount(); i++) {
    var projectileHandle = gethandle("projectile" + i);

    if (projectileHandle.sourceType.includes("enemy")) {
        move(projectileHandle, 0, +5);
    }
    else if (projectileHandle.sourceType.includes("character")) {
        move(projectileHandle, 0, -5);
    }
}

