import * as fluence from 'fluence';

async function main() {
    try {
        let appId = '1';
        const session = fluence.directConnect("localhost", 30000, appId);
        const result = await session.request("Test");
        console.log(result.asString());
    } catch (err) {
        console.error(err);
    }
}

main();
