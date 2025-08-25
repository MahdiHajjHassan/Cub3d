#!/bin/bash

# Cub3D Theme Switcher Script
# Switches between original and cybersecurity themes

ASSETS_DIR="assets"
BACKUP_DIR="assets/backup_original"

show_help() {
    echo "Cub3D Theme Switcher"
    echo "Usage: $0 [cyber|original|status]"
    echo ""
    echo "Commands:"
    echo "  cyber     - Switch to cybersecurity theme"
    echo "  original  - Switch back to original theme"
    echo "  status    - Show current theme status"
    echo "  help      - Show this help message"
}

backup_original() {
    if [ ! -d "$BACKUP_DIR" ]; then
        echo "Creating backup of original textures..."
        mkdir -p "$BACKUP_DIR"
        cp "$ASSETS_DIR/wall_north.xpm" "$BACKUP_DIR/"
        cp "$ASSETS_DIR/wall_south.xpm" "$BACKUP_DIR/"
        cp "$ASSETS_DIR/wall_east.xpm" "$BACKUP_DIR/"
        cp "$ASSETS_DIR/wall_west.xpm" "$BACKUP_DIR/"
    fi
}

switch_to_cyber() {
    echo "🔧 Switching to Cybersecurity Theme..."
    
    backup_original
    
    # Check if cyber textures exist
    if [ ! -f "$ASSETS_DIR/cyber_wall_north.xpm" ]; then
        echo "❌ Cyber textures not found! Please ensure cyber_wall_*.xpm files exist."
        exit 1
    fi
    
    # Copy cyber textures to main texture names
    cp "$ASSETS_DIR/cyber_wall_north.xpm" "$ASSETS_DIR/wall_north.xpm"
    cp "$ASSETS_DIR/cyber_wall_south.xpm" "$ASSETS_DIR/wall_south.xpm"
    cp "$ASSETS_DIR/cyber_wall_east.xpm" "$ASSETS_DIR/wall_east.xpm"
    cp "$ASSETS_DIR/cyber_wall_west.xpm" "$ASSETS_DIR/wall_west.xpm"
    
    echo "✅ Cybersecurity theme activated!"
    echo "🎮 You can now run: ./cub3d maps/bonus.cub"
    echo "🌆 Or try the complex map: ./cub3d maps/cyber_complex.cub"
}

switch_to_original() {
    echo "🔄 Switching back to Original Theme..."
    
    if [ ! -d "$BACKUP_DIR" ]; then
        echo "❌ Original texture backup not found!"
        exit 1
    fi
    
    # Restore original textures
    cp "$BACKUP_DIR/wall_north.xpm" "$ASSETS_DIR/"
    cp "$BACKUP_DIR/wall_south.xpm" "$ASSETS_DIR/"
    cp "$BACKUP_DIR/wall_east.xpm" "$ASSETS_DIR/"
    cp "$BACKUP_DIR/wall_west.xpm" "$ASSETS_DIR/"
    
    echo "✅ Original theme restored!"
    echo "🎮 You can now run: ./cub3d maps/bonus.cub"
}

show_status() {
    echo "🔍 Theme Status Check"
    echo "===================="
    
    if [ -f "$ASSETS_DIR/cyber_wall_north.xpm" ]; then
        echo "✅ Cyber textures: Available"
    else
        echo "❌ Cyber textures: Missing"
    fi
    
    if [ -d "$BACKUP_DIR" ]; then
        echo "✅ Original backup: Available"
    else
        echo "❌ Original backup: Missing"
    fi
    
    echo ""
    echo "📁 Available maps:"
    echo "   • maps/bonus.cub (basic cyber theme)"
    echo "   • maps/cyber_complex.cub (advanced layout)"
    echo "   • maps/t_ok.cub (simple test map)"
}

case "$1" in
    "cyber")
        switch_to_cyber
        ;;
    "original")
        switch_to_original
        ;;
    "status")
        show_status
        ;;
    "help"|"--help"|"-h")
        show_help
        ;;
    *)
        echo "❓ Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac
