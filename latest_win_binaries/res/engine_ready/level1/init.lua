-- require append_base_path("common.lua")

LEVEL_ASSETS = {

}

function crate_factory()
    local e = create_entity()
    set_mesh(e, "global/crate/crate.dae")
    return e
end

function crystal_factory()
    local e = create_entity()
    set_mesh(e, "global/collectables/crystal.dae")
    return e
end

function gem_factory()
    local e = create_entity()
    set_mesh(e, "global/collectables/blue_gem.dae")
    return e
end

function add_skybox()
    local e = create_entity()
    set_mesh(e, "global/skybox/skybox.dae")
    e = scale(e, 20)
    return e
end

function add_level_start_room()
    local e = create_entity()
    set_mesh(e, "level_start_room/level_start_room.dae")
    return e
end

function add_bush()
    local e = create_entity()
    set_mesh(e, "decor/bush.dae")
    return e
end

function add_big_rock()
    local e = create_entity()
    set_mesh(e, "decor/big_rock.dae")
    return e
end

function add_chest()
    local e = create_entity()
    set_mesh(e, "global/chest/chest.dae")
    return e
end

-- main --
--local e = gem_factory()
--translate(e, -5, -5, 0)
add_skybox()
--add_level_start_room()

e = add_chest()
scale(e, 1)
translate(e, 10, 10, 0)

e = add_big_rock()
translate(e, 10, 15, 0)

local e = crate_factory()
translate(e, 10, 0, 0)
scale(e, 0.1)
set_player(e)
