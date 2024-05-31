import requests
max_page = 10
urls = []
proj_id = {'dev':['groups',64],
            'jmli':['users',42],
            'qs':['groups',68],
            'qr':['groups',66],
            'wheel':['groups',63],
            'pub':['groups',62],
            'statarb':['groups',7]
        }
for proj, id in proj_id.items():
    for p in range(1,max_page):
        try:
            url=f'https://git.abcap.io//api/v4/{id[0]}/{id[1]}/projects?per_page=100&page={p}'
            headers = {
                'Private-Token': "glpat-U8gkqaW2KXzyEt3gMrZB"
            }
            response = requests.get(url, headers=headers)
            if (len(response.json()) == 0):
                break
            for i in response.json():
                urls.append(i['http_url_to_repo'])
        except Exception as e:
            print(e)
            print(url)
            print(i)
            continue
with open('./gen_urls.txt','w') as f:
    for url in urls:
        f.write(url+'\n')
