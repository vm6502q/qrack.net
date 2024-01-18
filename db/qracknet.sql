--
-- PostgreSQL database dump
--

-- Dumped from database version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)

-- Started on 2024-01-18 09:42:35 EST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 206 (class 1259 OID 18016)
-- Name: jobStatusTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."jobStatusTypes" (
    id integer NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."jobStatusTypes" OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 17972)
-- Name: jobs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.jobs (
    id integer NOT NULL,
    "jobStatusTypeId" integer NOT NULL,
    "userId" integer NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.jobs OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 17975)
-- Name: outputTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."outputTypes" (
    id integer NOT NULL,
    name text NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public."outputTypes" OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 17981)
-- Name: outputs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.outputs (
    id integer NOT NULL,
    "jobId" integer NOT NULL,
    "outputTypeId" integer NOT NULL,
    name text NOT NULL,
    value text NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.outputs OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 17987)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    username text NOT NULL,
    "usernameNormal" text NOT NULL,
    "passwordHash" text NOT NULL,
    affiliation text NOT NULL,
    name text NOT NULL,
    email text NOT NULL,
    "recoveryToken" text NOT NULL,
    "recoveryTokenExpiration" date NOT NULL,
    "createdAt" date NOT NULL,
    "updatedAt" date
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 2992 (class 0 OID 18016)
-- Dependencies: 206
-- Data for Name: jobStatusTypes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."jobStatusTypes" (id, name) FROM stdin;
1	SUCCESS
2	FAILED
3	RUNNING
4	IN QUEUE
\.


--
-- TOC entry 2849 (class 2606 OID 17994)
-- Name: jobs jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_pkey PRIMARY KEY (id);


--
-- TOC entry 2851 (class 2606 OID 17996)
-- Name: outputTypes outputTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."outputTypes"
    ADD CONSTRAINT "outputTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2853 (class 2606 OID 17998)
-- Name: outputs outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_pkey PRIMARY KEY (id);


--
-- TOC entry 2857 (class 2606 OID 18023)
-- Name: jobStatusTypes statusTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."jobStatusTypes"
    ADD CONSTRAINT "statusTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2855 (class 2606 OID 18000)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2859 (class 2606 OID 18024)
-- Name: jobs jobs_jobstatustypeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_jobstatustypeid_fk FOREIGN KEY ("jobStatusTypeId") REFERENCES public."jobStatusTypes"(id) NOT VALID;


--
-- TOC entry 2858 (class 2606 OID 18001)
-- Name: jobs jobs_userid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_userid_fk FOREIGN KEY ("userId") REFERENCES public.users(id) ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2860 (class 2606 OID 18006)
-- Name: outputs outputs_jobid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_jobid_fk FOREIGN KEY ("jobId") REFERENCES public.jobs(id) NOT VALID;


--
-- TOC entry 2861 (class 2606 OID 18011)
-- Name: outputs outputs_typeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_typeid_fk FOREIGN KEY ("outputTypeId") REFERENCES public."outputTypes"(id) NOT VALID;


-- Completed on 2024-01-18 09:42:35 EST

--
-- PostgreSQL database dump complete
--

