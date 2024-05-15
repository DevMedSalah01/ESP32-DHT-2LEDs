const { graphqlHTTP } = require('express-graphql');
const { makeExecutableSchema } = require('graphql-tools');
const { execute, subscribe } = require('graphql');
const { SubscriptionServer } = require('subscriptions-transport-ws');
const express = require('express');
const { createServer } = require('http');
const mqtt = require('mqtt');
const { PubSub, withFilter } = require('graphql-subscriptions');
const { buildSchema } = require('graphql');

const typeDefs = `
  type Query {
    hello: String!
    getLEDState: Boolean
  }

  type Mutation {
    send(message: String!): String!
    setLEDState(state: Boolean): Boolean
  }
  
  type Subscription {
    message: String!
    ledState: Boolean
  }
`;

const resolvers = {
  Query: {
    hello: () => 'Hello GraphQL!',
    getLEDState: () => {
      return ledState;
    },
  },
  Mutation: {
    send: (parent, { message }, context) => {
      const { pubsub } = context;
      // Publish the received message via MQTT to the GraphQL channel
      pubsub.publish('MESSAGE_RECEIVED', { message });
      return 'Message sent successfully!';
    },
    setLEDState: (parent, { state }, context) => {
      const { pubsub } = context;
      ledState = state;
      pubsub.publish('LED_STATE_CHANGED', { ledState });
      return ledState;
    },
  },
  Subscription: {
    message: {
      subscribe: (parent, args, context) => {
        const { pubsub } = context;
        const mqttClient = mqtt.connect('mqtt://localhost');
        mqttClient.on('message', (topic, message) => {
          // Publish the received message via MQTT to the GraphQL channel
          pubsub.publish('MESSAGE_RECEIVED', { message: message.toString() });
        });
        // Return the GraphQL channel generator
        return pubsub.asyncIterator('MESSAGE_RECEIVED');
      },
    },
    ledState: {
      subscribe: (_, __, context) => {
        const { pubsub } = context;
        return pubsub.asyncIterator('LED_STATE_CHANGED');
      },
    },
  },
};

const pubsub = new PubSub();
const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
});

const app = express();
const server = createServer(app);

// Define the GraphQL endpoint route
app.use('/graphql', express.json(), graphqlHTTP({
  schema,
  graphiql: true, // Enable the GraphQL Playground
  context: { pubsub }, // Provide the pubsub object to the resolver context
}));

server.listen(4000, () => {
  console.log('Server is running on http://localhost:4000');
  new SubscriptionServer(
    {
      execute,
      subscribe,
      schema,
    },
    {
      server,
      path: '/subscriptions',
    }
  );
});