#!/usr/bin/env python3
import re

# Read the file
with open('/Users/genius/project/opendarkeden/client/Client/MTopView.cpp', 'r') as f:
    content = f.read()

# Pattern to match GDI_Text calls
pattern = r'(\s*)(pSurfaceCast->GDI_Text\(\s*(\d+),\s*(\d+),\s*([^,]+),\s*RGB\([^)]+\)\s*\);)'

def replace_gdi_text(match):
    indent = match.group(1)
    original_call = match.group(2)
    x = match.group(3)
    y = match.group(4)
    text_var = match.group(5)
    
    # Return the replacement
    return f'{indent}// {original_call}\n{indent}TextSystem::TextService::RenderText({x}, {y}, {text_var});'

# Replace all GDI_Text calls
new_content = re.sub(pattern, replace_gdi_text, content)

# Write the file back
with open('/Users/genius/project/opendarkeden/client/Client/MTopView.cpp', 'w') as f:
    f.write(new_content)

print("GDI_Text calls replaced with TextSystem::RenderText in MTopView.cpp")