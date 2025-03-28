#include <room_generator.h>
#include <room.h>

void RoomGenerator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_player", "path"), &RoomGenerator::set_player);
    ClassDB::bind_method(D_METHOD("get_player"), &RoomGenerator::get_player);
    ClassDB::bind_method(D_METHOD("set_tile_map", "path"), &RoomGenerator::set_tile_map);
    ClassDB::bind_method(D_METHOD("get_tile_map"), &RoomGenerator::get_tile_map);

    ClassDB::bind_method(D_METHOD("set_room_size_x", "size"), &RoomGenerator::set_room_size_x);
    ClassDB::bind_method(D_METHOD("get_room_size_x"), &RoomGenerator::get_room_size_x);
    ClassDB::bind_method(D_METHOD("set_room_size_y", "size"), &RoomGenerator::set_room_size_y);
    ClassDB::bind_method(D_METHOD("get_room_size_y"), &RoomGenerator::get_room_size_y);
    ClassDB::bind_method(D_METHOD("set_room_quantity", "quantity"), &RoomGenerator::set_room_quantity);
    ClassDB::bind_method(D_METHOD("get_room_quantity"), &RoomGenerator::get_room_quantity);
    ClassDB::bind_method(D_METHOD("get_rooms"), &RoomGenerator::get_rooms);
    ClassDB::bind_method(D_METHOD("set_remove_entire_wall", "new_value"), &RoomGenerator::set_remove_entire_wall);
    ClassDB::bind_method(D_METHOD("get_remove_entire_wall"), &RoomGenerator::get_remove_entire_wall);

    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player"), "set_player", "get_player");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "tile_map"), "set_tile_map", "get_tile_map");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "room_size_x"), "set_room_size_x", "get_room_size_x");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "room_size_y"), "set_room_size_y", "get_room_size_y");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "room_quantity"), "set_room_quantity", "get_room_quantity");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "remove_entire_wall"), "set_remove_entire_wall", "get_remove_entire_wall");
}

RoomGenerator::RoomGenerator(){

}

void RoomGenerator::_ready(){
    generate_rooms();
}

void RoomGenerator::generate_rooms(){
    //TileMapLayer *tile_map = Object::cast_to<TileMapLayer>(get_node<Node>(tile_map_path));

    for (int a = 0; a < room_quantity; a++) {
        Array row;
        for (int b = 0; b < room_quantity; b++) {
            Vector2i glob_pos = Vector2i(a * room_size_x * 16, b * room_size_y * 16);
            Vector2i tile_pos = Vector2i(a, b);
            create_one_room(glob_pos, tile_pos);
            //Ref<Room> room = Room(glob_pos, tile_pos);
            Ref<Room> room;
            room.instantiate();
            room->set_glob_pos(glob_pos);
            room->set_tile_pos(tile_pos);
            row.append(room);
        }
        rooms.append(row);
    }

    create_maze();

}

void RoomGenerator::create_maze(){
    Array stack;
    Vector2i current_element = Vector2i(0, 0);
    Array fist_row = rooms[current_element.x];
    Ref<Room> first_room =fist_row[current_element.y];
    first_room->set_visited(true);
    
    stack.append(first_room);

    while(!stack.is_empty()){
        Array directions = Array::make(Vector2i(1, 0), Vector2i(-1, 0), Vector2i(0, 1), Vector2i(0, -1));
        directions.shuffle();
        bool found_unvisited = false;

        for (int a = 0; a < directions.size(); a++){
            Vector2i neighbor = current_element + directions[a];

            if(neighbor.x < 0 || neighbor.x >= room_quantity || neighbor.y < 0 || neighbor.y >= room_quantity){
                continue;
            }

            Array fist_row_2 = rooms[neighbor.x];
            Ref<Room> neighbor_room = fist_row_2[neighbor.y];

            if (!neighbor_room->get_visited()){
                Ref<Room> current_room = stack[stack.size() - 1];
                remove_wall(current_room->get_glob_pos(), directions[a]);
                neighbor_room->set_visited(true);
                current_element = neighbor;
                stack.append(neighbor_room);
                found_unvisited = true;
                printf("find new\n");
            }
        }

        if(!found_unvisited){
            stack.pop_back();
            if(!stack.is_empty()){
                Ref<Room> last_room = stack[stack.size() - 1];
                current_element = last_room->get_tile_pos();
            }
        }

    }

}


void RoomGenerator::remove_wall(Vector2i pos, Vector2i dir){
    // remove a wall, can create only the door passage, or remove entire wall
    
    TileMapLayer *tile_map = Object::cast_to<TileMapLayer>(get_node<Node>(tile_map_path));

    //left
    if(dir == Vector2i(-1, 0)){
        if(remove_entire_wall)
        for(int i = 1; i < room_size_y; i++){
            tile_map->set_cell(pos/16 + Vector2i(0, -i), -1, Vector2i(1, 1));
        } else{
            tile_map->set_cell(pos/16 + Vector2i(0, -room_size_y/2), -1, Vector2i(1, 1));
        }

    } else if (dir == Vector2i(1, 0)){
        //right
        if(remove_entire_wall)
        for(int i = 1; i < room_size_y; i++){
            tile_map->set_cell(pos/16 + Vector2i(room_size_x, -i), -1, Vector2i(1, 1));
        } else{
            tile_map->set_cell(pos/16 + Vector2i(room_size_x, -room_size_y/2), -1, Vector2i(1, 1));
        }
    } else if (dir == Vector2i(0, 1)){
        //down
        if(remove_entire_wall)
        for(int i = 1; i < room_size_x; i++){
            tile_map->set_cell(pos/16 + Vector2i(i, 0), -1, Vector2i(1, 1));
        } else{
            tile_map->set_cell(pos/16 + Vector2i(room_size_x/2, 0), -1, Vector2i(1, 1));
        }
    }  else if (dir == Vector2i(0, -1)){
        //up
        if(remove_entire_wall)
        for(int i = 1; i < room_size_x; i++){
            tile_map->set_cell(pos/16 + Vector2i(i, -room_size_y), -1, Vector2i(1, 1));
        } else{
            tile_map->set_cell(pos/16 + Vector2i(room_size_x/2, -room_size_y), -1, Vector2i(1, 1));
        }
    }
}


void RoomGenerator::create_one_room(Vector2i glob_pos, Vector2i tile_pos){
    TileMapLayer *tile_map = Object::cast_to<TileMapLayer>(get_node<Node>(tile_map_path));

    
    // Draw walls
    for (int x = 0; x <= room_size_x; x++) {
        tile_map->set_cell(glob_pos / 16 + Vector2i(x, 0), 0, Vector2i(1, 1));
        tile_map->set_cell(glob_pos / 16 + Vector2i(x, -room_size_y), 0, Vector2i(1, 1));
    }
    
    for (int y = -room_size_y + 1; y <= 0; y++) {
        tile_map->set_cell(glob_pos / 16 + Vector2i(0, y), 0, Vector2i(1, 1));
        tile_map->set_cell(glob_pos / 16 + Vector2i(room_size_x, y), 0, Vector2i(1, 1));
    }
    
    
    return;
}
