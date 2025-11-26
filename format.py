


with open("torch_positions.txt","r",encoding="utf-8") as fp:
    code="std::vector<glm::vec3> coords={\n"
    for line in fp:
        line=line.replace('\n','')
        code+=f'    {{{line}}},\n'
    code+='};'
    print(code)