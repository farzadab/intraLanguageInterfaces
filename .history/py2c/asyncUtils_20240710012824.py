import asyncio
import aiohttp

async def fetch_data(url):
    async with aiohttp.ClientSession() as session:
        async with session.get(url) as response:
            return await response.text()

async def fetch_multiple(urls):
    async with aiohttp.ClientSession() as session:
        tasks = [fetch_data(url) for url in urls]
        return await asyncio.gather(*tasks)

async def post_data(url, data):
    async with aiohttp.ClientSession() as session:
        async with session.post(url, json=data) as response:
            return await response.text()

async def fetch_with_timeout(url, timeout):
    async with aiohttp.ClientSession() as session:
        try:
            async with session.get(url, timeout=timeout) as response:
                return await response.text()
        except asyncio.TimeoutError:
            return "Request timed out"


# config for post_data
# {
#     "python_env": "/Users/visheshyadav/anaconda3",
#     "module_name": "asyncUtils",
#     "function_name": "post_data",
#     "args": ["https://jsonplaceholder.typicode.com/posts", {"title": "foo", "body": "bar", "userId": 1}]
# }

# config for fetch_with_timeout
# {
#     "python_env": "/Users/visheshyadav/anaconda3",
#     "module_name": "asyncUtils",
#     "function_name": "fetch_with_timeout",
#     "args": ["https://jsonplaceholder.typicode.com/todos/1", 5]
# }